use std::collections::HashSet;
use std::ffi::CStr;
use std::fs::{File, OpenOptions};
use std::io::{BufRead, BufReader, Write};
use std::os::raw::c_char;
use std::path::PathBuf;
use std::sync::OnceLock;

static mut BLOCKED_DOMAINS: *const HashSet<&'static str> = std::ptr::null();
static ERROR_LOG: OnceLock<File> = OnceLock::new();

fn log_error(message: &str) {
    if let Some(mut error_log) = ERROR_LOG.get() {
        if let Err(e) = error_log.write(message.as_bytes()) {
            eprintln!("shimmy: {}", message);
            eprintln!("shimmy: Failed to write to error log: {}", e);
        }

        #[cfg(target_family = "unix")]
        {
            if let Err(e) = error_log.write(b"\n") {
                eprintln!("shimmy: {}", message);
                eprintln!("shimmy: Failed to write to error log: {}", e);
            }
        }

        #[cfg(target_family = "windows")]
        {
            if let Err(e) = error_log.write(b"\r\n") {
                eprintln!("shimmy: {}", message);
                eprintln!("shimmy: Failed to write to error log: {}", e);
            }
        }
    } else {
        eprintln!("shimmy: {}", message);
    }
}

#[cold]
#[no_mangle]
pub extern "C" fn shmy_init() {
    // Safety: This function is only called once and is called before BLOCKED_DOMAINS is accessed, so
    // it is safe to modify BLOCKED_DOMAINS.

    if unsafe { !BLOCKED_DOMAINS.is_null() } {
        log_error("shimmy: shmy_init() called more than once");
        return;
    }

    let mut map: Box<HashSet<&'static str>> = Box::new(HashSet::new());

    let data_dir = {
        #[cfg(target_family = "unix")]
        {
            let Ok(home) = std::env::var("HOME") else {
                log_error(&format!("$HOME environment variable not set"));
                return;
            };

            let mut path = PathBuf::from(home);
            path.push(".shimmy");

            path
        }

        #[cfg(target_family = "windows")]
        {
            let Ok(appdata) = std::env::var("APPDATA") else {
                log_error(&format!("%APPDATA% environment variable not set"));
                return;
            };

            let mut path = PathBuf::from(appdata);
            path.push("Local\\Shimmy");

            path
        }
    };

    if !data_dir.exists() {
        if let Err(e) = std::fs::create_dir_all(&data_dir) {
            log_error(&format!(
                "Failed to create data directory '{}': {}",
                data_dir.display(),
                e
            ));
            return;
        }
    }

    let error_log_path = data_dir.join("error.log");
    let error_log_file = match OpenOptions::new()
        .write(true)
        .create(true)
        .append(true)
        .open(&error_log_path)
    {
        Ok(file) => file,
        Err(e) => {
            log_error(&format!(
                "Failed to create error log file '{}': {}",
                error_log_path.display(),
                e
            ));
            return;
        }
    };

    if ERROR_LOG.set(error_log_file).is_err() {
        log_error("Failed to save reference to error log file");
        return;
    }

    let blocked_domains_path = data_dir.join("blocked_domains.txt");
    let blocked_domains_file = match File::open(&blocked_domains_path) {
        Ok(file) => file,
        Err(e) => {
            log_error(&format!(
                "Failed to open blocked domains file '{}': {}",
                blocked_domains_path.display(),
                e
            ));
            return;
        }
    };

    let reader = BufReader::new(blocked_domains_file);

    for line in reader.lines() {
        let line = match line {
            Ok(line) => line,
            Err(e) => {
                log_error(&format!(
                    "Failed to read blocked domains file '{}': {}",
                    blocked_domains_path.display(),
                    e
                ));
                return;
            }
        };

        let mut domain_parts = line.split('#');
        let domain = domain_parts.next().unwrap_or("").trim();
        if domain.is_empty() {
            continue;
        }

        let domain = Box::leak(domain.to_owned().into_boxed_str());
        map.insert(domain);
    }

    unsafe { BLOCKED_DOMAINS = Box::leak(map) as *const _ };
    println!("shimmy: initialized");
}

/// Returns 0 if not found, 1 if found
#[no_mangle]
pub extern "C" fn shmy_is_domain_blocked(domain: *const c_char) -> u8 {
    if domain.is_null() {
        return 0;
    }

    let domain = unsafe { CStr::from_ptr(domain) };
    let Ok(domain) = domain.to_str() else {
        return 0;
    };

    let blocked_domains = unsafe { &*BLOCKED_DOMAINS };

    return blocked_domains.contains(domain) as u8;
}
