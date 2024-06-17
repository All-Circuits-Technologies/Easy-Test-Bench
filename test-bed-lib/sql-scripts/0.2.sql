-- SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
-- SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
--
-- SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

DROP TABLE IF EXISTS db_version$

CREATE TABLE IF NOT EXISTS libOptions(
    id INTEGER PRIMARY KEY,

    dbVersion TEXT DEFAULT 0,
    guestModeAllowed INTEGER DEFAULT 1,
    lastModified INTEGER DEFAULT 0,
    CHECK (guestModeAllowed IN (0, 1))
)$

INSERT OR IGNORE INTO libOptions (dbVersion) VALUES ('0.2')$

CREATE TABLE IF NOT EXISTS projects(
    id INTEGER PRIMARY KEY,
    name TEXT,
    path TEXT,
    lastOpened INTEGER DEFAULT 0,
    lastModified INTEGER DEFAULT 0
)$

CREATE TRIGGER set_default_projects_lastmodified
AFTER INSERT ON projects
BEGIN
    UPDATE projects
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_projects_lastmodified
   BEFORE UPDATE OF id, name, path, lastOpened ON projects
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_projects_lastmodified
        AFTER UPDATE ON projects
        BEGIN
   UPDATE projects
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$

ALTER TABLE users
        ADD lastModified INTEGER DEFAULT 0$

CREATE TRIGGER set_default_users_lastmodified
AFTER INSERT ON users
BEGIN
    UPDATE users
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_users_lastmodified
   BEFORE UPDATE OF id, lastName, firstName, username, password, salt, roles ON users
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_users_lastmodified
        AFTER UPDATE ON users
        BEGIN
   UPDATE users
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$

ALTER TABLE roles
        ADD lastModified INTEGER DEFAULT 0$

CREATE TRIGGER set_default_role_lastmodified
AFTER INSERT ON roles
BEGIN
    UPDATE roles
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_roles_lastmodified
   BEFORE UPDATE OF id, name ON roles
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_roles_lastmodified
        AFTER UPDATE ON roles
        BEGIN
   UPDATE roles
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$

ALTER TABLE permissions
        ADD lastModified INTEGER DEFAULT 0$

CREATE TRIGGER set_default_permissions_lastmodified
AFTER INSERT ON permissions
BEGIN
    UPDATE permissions
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_permissions_lastmodified
   BEFORE UPDATE OF id, name, description ON permissions
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_permissions_lastmodified
        AFTER UPDATE ON permissions
        BEGIN
   UPDATE permissions
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$

ALTER TABLE permissionRoles
        ADD lastModified INTEGER DEFAULT 0$

CREATE TRIGGER set_default_permissionRoles_lastmodified
AFTER INSERT ON permissionRoles
BEGIN
    UPDATE permissionRoles
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_permissionRoles_lastmodified
   BEFORE UPDATE OF id, permissionName, roleName ON permissionRoles
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_permissionRoles_lastmodified
        AFTER UPDATE ON permissionRoles
        BEGIN
   UPDATE permissionRoles
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$

UPDATE pluginPermanentStorage
SET lastModified = lastModified/1000$

CREATE TRIGGER set_default_pluginPermanentStorage_lastmodified
AFTER INSERT ON pluginPermanentStorage
BEGIN
    UPDATE pluginPermanentStorage
    SET lastModified = strftime('%s', datetime('now'))
    WHERE Id = NEW.Id;
END$

CREATE TRIGGER check_pluginPermanentStorage_lastmodified
   BEFORE UPDATE OF id, key, value, note ON pluginPermanentStorage
BEGIN
   SELECT
      CASE
        WHEN OLD.lastModified <> NEW.lastModified THEN
          RAISE (ABORT,'Update conflict, the object has been modified in the meantime')
       END;
END$

CREATE TRIGGER update_pluginPermanentStorage_lastmodified
        AFTER UPDATE ON pluginPermanentStorage
        BEGIN
   UPDATE pluginPermanentStorage
SET
   lastModified = strftime('%s', datetime('now'))
WHERE
   id = NEW.id;
END$
