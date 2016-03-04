-- ======================================================================
-- QPF General Database Creation Procedure
-- J C Gonzalez
-- ======================================================================

-- Database: qpfdb

-- DROP DATABASE qpfdb;
-- CREATE DATABASE qpfdb
-- WITH OWNER = jcgonzalez
-- ENCODING = 'UTF8'
-- TABLESPACE = pg_default
-- LC_COLLATE = 'en_US.UTF-8'
-- LC_CTYPE = 'en_US.UTF-8'
-- CONNECTION LIMIT = -1;
-- GRANT CONNECT, TEMPORARY ON DATABASE qpfdb TO public;
-- GRANT ALL ON DATABASE qpfdb TO jcgonzalez;

-- Role: jcgonzalez

-- DROP ROLE jcgonzalez;
-- CREATE ROLE jcgonzalez LOGIN
-- SUPERUSER INHERIT CREATEDB NOCREATEROLE NOREPLICATION;

-- Tables

DROP TABLE IF EXISTS alarms CASCADE;
DROP TABLE IF EXISTS config_general CASCADE;
DROP TABLE IF EXISTS config_nodes CASCADE;
DROP TABLE IF EXISTS config_orchestration CASCADE;
DROP TABLE IF EXISTS config_processors CASCADE;
DROP TABLE IF EXISTS config_products CASCADE;
DROP TABLE IF EXISTS creators CASCADE;
DROP TABLE IF EXISTS instruments CASCADE;
DROP TABLE IF EXISTS message_type CASCADE;
DROP TABLE IF EXISTS messages CASCADE;
DROP TABLE IF EXISTS transmissions CASCADE;
DROP TABLE IF EXISTS observation_modes CASCADE;
DROP TABLE IF EXISTS product_status CASCADE;
DROP TABLE IF EXISTS products_info CASCADE;
DROP TABLE IF EXISTS task_inputs CASCADE;
DROP TABLE IF EXISTS task_outputs CASCADE;
DROP TABLE IF EXISTS task_status CASCADE;
DROP TABLE IF EXISTS tasks_info CASCADE;
DROP TABLE IF EXISTS variables CASCADE;

-- ====[ Configuration ]==========
CREATE TABLE config_general (
    param_name varchar(32) NOT NULL,
    content varchar(256),
    PRIMARY KEY (param_name)
  );

CREATE TABLE config_nodes (
    node_name varchar(20) NOT NULL,
    node_type varchar(20),
    client_addr varchar(128),
    server_addr varchar(128),
    PRIMARY KEY (node_name)
  );

CREATE TABLE config_processors (
    processor_id int NOT NULL,
    processor_name varchar(1024),
    exe_path varchar(1024),
    input_path varchar(1024),
    output_path varchar(1024),
    PRIMARY KEY (processor_id)
  );

CREATE TABLE config_orchestration (
    rule_id int NOT NULL,
    rule_name varchar(64),
    inputs varchar(1024),
    outputs varchar(1024),
    processor_id int REFERENCES config_processors,
    PRIMARY KEY (rule_id)
  );

CREATE TABLE config_products (
    product_type varchar(128) NOT NULL,
    PRIMARY KEY (product_type)
  );

-- ====[ Products ]==========
CREATE TABLE product_status (
    product_status_id int NOT NULL,
    status_desc varchar(128),
    PRIMARY KEY (product_status_id)
  );

INSERT INTO product_status (product_status_id, status_desc) VALUES
    ( 0, 'OK' ),
    ( 1, 'NOTOK' );

CREATE TABLE instruments (
   instrument_id int NOT NULL,
   instrument varchar(128),
   PRIMARY KEY (instrument_id)
  );

INSERT INTO instruments (instrument_id, instrument) VALUES
    ( 1, 'VIS' ),
    ( 2, 'NISP.P' ),
    ( 3, 'NISP.S' );

CREATE TABLE creators (
    creator_id int NOT NULL,
    creator_desc varchar(128),
    PRIMARY KEY (creator_id)
  );

INSERT INTO creators (creator_id, creator_desc) VALUES
    ( 1, 'SOC_L1P' ),
    ( 10, 'SOC_QLA.TEST' ),
    ( 11, 'SOC_QLA.OPE' );

CREATE TABLE observation_modes (
    obsmode_id int NOT NULL,
    obsmode_desc varchar(128),
    PRIMARY KEY (obsmode_id)
  );

INSERT INTO observation_modes (obsmode_id, obsmode_desc) VALUES
       ( 1, 'NOMINAL' ),
       ( 2, 'TEST' );

CREATE TABLE products_info (
    id serial NOT NULL,
    product_id varchar(256) NOT NULL,
    product_type varchar(128) REFERENCES config_products,
    product_version varchar(128),
    product_size int,
    product_status_id int REFERENCES product_status(product_status_id),
    creator_id int REFERENCES creators,
    instrument_id int REFERENCES instruments,
    obsmode_id int NOT NULL,
    start_time timestamp,
    end_time timestamp,
    registration_time timestamp,
    url varchar(1024),
    PRIMARY KEY (product_id)
  );

-- ====[ Tasks ]==========
CREATE TABLE task_status (
    task_status_id int NOT NULL,
    status_desc varchar(128),
    PRIMARY KEY (task_status_id)
  );

INSERT INTO task_status (task_status_id, status_desc) VALUES
    ( -1, 'FAILED'),
    ( 0, 'FINISHED' ),
    ( 1, 'RUNNING' );

CREATE TABLE tasks_info (
    id serial NOT NULL,
    task_id varchar(128) NOT NULL,
    task_status_id int REFERENCES task_status(task_status_id),
    task_exitcode int,
    task_path varchar(1024),
    task_size int,
    registration_time timestamp,
    start_time timestamp,
    end_time timestamp,
    task_data json,
    PRIMARY KEY (task_id)
  );

CREATE TABLE task_inputs (
    task_id varchar(128) REFERENCES tasks_info(task_id),
    product_id varchar(256) NOT NULL,
    PRIMARY KEY (task_id,product_id)
  );

CREATE TABLE task_outputs (
    task_id varchar(128) REFERENCES tasks_info(task_id),
    product_id varchar(256) NOT NULL,
    PRIMARY KEY (task_id,product_id)
  );

-- ====[ Messages ]==========
CREATE TABLE messages (
    id serial NOT NULL,
    message_id varchar(128) NOT NULL,
    message_type varchar(128) NOT NULL,
    origin varchar(128) NOT NULL,
    destination varchar(128) NOT NULL,
    message_version int,
    creation_time timestamp,
    transmission_time timestamp,
    reception_time timestamp,
    PRIMARY KEY (id,message_id,message_type,origin,destination)
  );

CREATE TABLE message_type (
    message_type int NOT NULL,
    type_desc varchar(128),
    PRIMARY KEY (message_type)
  );

-- ====[ Transmissions ]=========

CREATE TABLE transmissions (
    id serial NOT NULL,
    msg_date timestamp without time zone,
    msg_from character(64),
    msg_to character(64),
    msg_type character(64),
    msg_bcast character(1),
    msg_content text,
    CONSTRAINT transmissions_pkey PRIMARY KEY (id)
  );

-- ====[ Node States ]=========

CREATE TABLE node_states (
    node_name varchar(20) NOT NULL,
    node_state varchar(32),
    PRIMARY KEY (node_name)
  );

-- ====[ Alarms ]==========
CREATE TABLE variables (
   var_name         varchar(50), -- Short name
   var_type         int,           -- Variable Type
   description     varchar(200),     -- Variable description
   smin            numeric,       -- Soft minimum value
   smax            numeric,       -- Soft maximum value
   hmin            numeric,       -- Hard minimum value
   hmax            numeric,       -- Hard maximum value
   x               numeric,       -- Current value
   PRIMARY KEY (var_name)
  );

CREATE TABLE alarms (
   alarm_id         serial, -- Alarm identifier
   var_name         varchar(50) REFERENCES variables,      -- Variable name
   origin          varchar(200),     -- Location in the code
   severity        integer,       -- Initial severity
   status          integer,       -- Initial status
   curr_severity    integer,       -- Current severity
   curr_status      integer,       -- Current status
   creation        timestamp,     -- Creation date
   acknowledged    timestamp,     -- Acknowledged date
   last_update      timestamp,     -- Last update
   alarm_state      integer,       -- Current state
   PRIMARY KEY (alarm_id)
  );
