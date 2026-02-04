INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES ('pi_compute', 'PI Computation', UNIX_TIMESTAMP());
INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES ('euler_compute', 'Euler Constant Computation', UNIX_TIMESTAMP());
INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES ('axb_compute', 'Ax=b Solver', UNIX_TIMESTAMP());
INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES ('collatz_search', 'Collatz Search', UNIX_TIMESTAMP());
INSERT IGNORE INTO platform (name, user_friendly_name, deprecated, create_time) VALUES ('x86_64-pc-linux-gnu', 'Linux x86-64', 0, UNIX_TIMESTAMP());
