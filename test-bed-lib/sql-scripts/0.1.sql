-- SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
-- SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
-- SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
--
-- SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

CREATE TABLE IF NOT EXISTS db_version(
    version TEXT
)$

CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY,
    firstName TEXT,
    lastName TEXT,
    username TEXT,
    password TEXT,
    salt TEXT,
    roles TEXT
)$

CREATE TABLE IF NOT EXISTS roles(
    id INTEGER PRIMARY KEY,
    name TEXT UNIQUE
)$

CREATE TABLE IF NOT EXISTS permissions(
    id INTEGER PRIMARY KEY,
    name TEXT UNIQUE,
    description TEXT
)$

CREATE TABLE IF NOT EXISTS permissionRoles(
    id INTEGER PRIMARY KEY,
    permissionName TEXT,
    roleName TEXT,
    FOREIGN KEY(permissionName) REFERENCES permissions(name),
    FOREIGN KEY(roleName) REFERENCES roles(name)
)$

CREATE TABLE IF NOT EXISTS pluginPermanentStorage(
    id INTEGER PRIMARY KEY,
    key TEXT,
    value TEXT,
    note TEXT,
    lastModified INTEGER
)$

INSERT OR IGNORE INTO db_version (version) VALUES (0.1)$

INSERT OR IGNORE INTO roles (id,name) VALUES ('0', 'default')$
INSERT OR IGNORE INTO roles (id,name) VALUES ('1', 'operator')$
INSERT OR IGNORE INTO roles (id,name) VALUES ('2', 'technician')$
INSERT OR IGNORE INTO roles (id,name) VALUES ('3', 'developer')$

INSERT OR IGNORE INTO users (id, firstName, lastName, username, password, salt, roles)
VALUES ('0', 'Anonymous', 'User', 'default','default','','default')$
