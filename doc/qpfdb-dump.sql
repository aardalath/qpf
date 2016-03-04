--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: alarms; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE alarms (
    alarm_id integer NOT NULL,
    var_name character(50),
    origin character(200),
    severity integer,
    status integer,
    curr_severity integer,
    curr_status integer,
    creation timestamp without time zone,
    acknowledged timestamp without time zone,
    last_update timestamp without time zone,
    alarm_state integer
);


ALTER TABLE alarms OWNER TO postgres;

--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE alarms_alarm_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE alarms_alarm_id_seq OWNER TO postgres;

--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE alarms_alarm_id_seq OWNED BY alarms.alarm_id;


--
-- Name: config_general; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config_general (
    param_name character(32) NOT NULL,
    content character(256)
);


ALTER TABLE config_general OWNER TO postgres;

--
-- Name: config_nodes; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config_nodes (
    node_name character(20) NOT NULL,
    node_type character(20),
    client_addr character(128),
    server_addr character(128)
);


ALTER TABLE config_nodes OWNER TO postgres;

--
-- Name: config_orchestration; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config_orchestration (
    rule_id integer NOT NULL,
    rule_name character(64),
    inputs character(1024),
    outputs character(1024),
    processor_id integer
);


ALTER TABLE config_orchestration OWNER TO postgres;

--
-- Name: config_processors; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config_processors (
    processor_id integer NOT NULL,
    processor_name character(1024),
    exe_path character(1024),
    input_path character(1024),
    output_path character(1024)
);


ALTER TABLE config_processors OWNER TO postgres;

--
-- Name: config_products; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config_products (
    product_type character(128) NOT NULL
);


ALTER TABLE config_products OWNER TO postgres;

--
-- Name: creators; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE creators (
    creator_id integer NOT NULL,
    creator_desc character(128)
);


ALTER TABLE creators OWNER TO postgres;

--
-- Name: instruments; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE instruments (
    instrument_id integer NOT NULL,
    instrument character(128)
);


ALTER TABLE instruments OWNER TO postgres;

--
-- Name: message_type; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE message_type (
    message_type integer NOT NULL,
    type_desc character(128)
);


ALTER TABLE message_type OWNER TO postgres;

--
-- Name: messages; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE messages (
    id integer NOT NULL,
    message_id character(128) NOT NULL,
    message_type character(128) NOT NULL,
    origin character(128) NOT NULL,
    destination character(128) NOT NULL,
    message_version integer,
    creation_time timestamp without time zone,
    transmission_time timestamp without time zone,
    reception_time timestamp without time zone
);


ALTER TABLE messages OWNER TO postgres;

--
-- Name: messages_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE messages_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE messages_id_seq OWNER TO postgres;

--
-- Name: messages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE messages_id_seq OWNED BY messages.id;


--
-- Name: observation_modes; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE observation_modes (
    obs_mode integer NOT NULL,
    obs_modedesc character(128)
);


ALTER TABLE observation_modes OWNER TO postgres;

--
-- Name: product_status; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE product_status (
    product_status integer NOT NULL,
    status_desc character(128)
);


ALTER TABLE product_status OWNER TO postgres;

--
-- Name: products_info; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE products_info (
    id integer NOT NULL,
    product_id character(256) NOT NULL,
    product_type character(128),
    product_status integer NOT NULL,
    product_version character(128),
    product_size integer,
    creator_id integer NOT NULL,
    instrument_id integer NOT NULL,
    obs_mode integer NOT NULL,
    start_time timestamp without time zone,
    end_time timestamp without time zone,
    registration_time timestamp without time zone,
    url character(1024)
);


ALTER TABLE products_info OWNER TO postgres;

--
-- Name: products_info_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE products_info_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE products_info_id_seq OWNER TO postgres;

--
-- Name: products_info_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE products_info_id_seq OWNED BY products_info.id;


--
-- Name: task_inputs; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE task_inputs (
    task_id character(128) NOT NULL,
    product_id character(256) NOT NULL
);


ALTER TABLE task_inputs OWNER TO postgres;

--
-- Name: task_outputs; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE task_outputs (
    task_id character(128) NOT NULL,
    product_id character(256) NOT NULL
);


ALTER TABLE task_outputs OWNER TO postgres;

--
-- Name: task_status; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE task_status (
    task_status integer NOT NULL,
    status_desc character(128)
);


ALTER TABLE task_status OWNER TO postgres;

--
-- Name: tasks_info; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tasks_info (
    id integer NOT NULL,
    task_id character(128) NOT NULL,
    task_status integer NOT NULL,
    task_exitcode integer,
    task_path character(1024),
    task_size integer,
    registration_time timestamp without time zone,
    start_time timestamp without time zone,
    end_time timestamp without time zone
);


ALTER TABLE tasks_info OWNER TO postgres;

--
-- Name: tasks_info_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tasks_info_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE tasks_info_id_seq OWNER TO postgres;

--
-- Name: tasks_info_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tasks_info_id_seq OWNED BY tasks_info.id;


--
-- Name: variables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE variables (
    var_name character(50) NOT NULL,
    var_type integer,
    description character(200),
    smin numeric,
    smax numeric,
    hmin numeric,
    hmax numeric,
    x numeric
);


ALTER TABLE variables OWNER TO postgres;

--
-- Name: alarm_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY alarms ALTER COLUMN alarm_id SET DEFAULT nextval('alarms_alarm_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY messages ALTER COLUMN id SET DEFAULT nextval('messages_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY products_info ALTER COLUMN id SET DEFAULT nextval('products_info_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tasks_info ALTER COLUMN id SET DEFAULT nextval('tasks_info_id_seq'::regclass);


--
-- Data for Name: alarms; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY alarms (alarm_id, var_name, origin, severity, status, curr_severity, curr_status, creation, acknowledged, last_update, alarm_state) FROM stdin;
\.


--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('alarms_alarm_id_seq', 1, false);


--
-- Data for Name: config_general; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config_general (param_name, content) FROM stdin;
app_name                        	QPF                                                                                                                                                                                                                                                             
app_version                     	0.1                                                                                                                                                                                                                                                             
last_access                     	20150616T121555                                                                                                                                                                                                                                                 
hmi_node                        	QPFHMI                                                                                                                                                                                                                                                          
\.


--
-- Data for Name: config_nodes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config_nodes (node_name, node_type, client_addr, server_addr) FROM stdin;
DataMng             	datamng             	tcp://127.0.0.1:7101                                                                                                            	tcp://*:7101                                                                                                                    
EvtMng              	evtmng              	tcp://127.0.0.1:7100                                                                                                            	tcp://*:7100                                                                                                                    
LogMng              	logmng              	tcp://127.0.0.1:7102                                                                                                            	tcp://*:7102                                                                                                                    
QPFHMI              	hmi                 	tcp://127.0.0.1:7999                                                                                                            	tcp://*:7999                                                                                                                    
TskAge1             	taskagent           	tcp://127.0.0.1:7111                                                                                                            	tcp://*:7111                                                                                                                    
TskAge2             	taskagent           	tcp://127.0.0.1:7112                                                                                                            	tcp://*:7112                                                                                                                    
TskAge3             	taskagent           	tcp://127.0.0.1:7113                                                                                                            	tcp://*:7113                                                                                                                    
TskAge4             	taskagent           	tcp://127.0.0.1:7114                                                                                                            	tcp://*:7114                                                                                                                    
TskAge5             	taskagent           	tcp://127.0.0.1:7115                                                                                                            	tcp://*:7115                                                                                                                    
TskMng              	taskmng             	tcp://127.0.0.1:7103                                                                                                            	tcp://*:7103                                                                                                                    
TskOrc              	taskorc             	tcp://127.0.0.1:7104                                                                                                            	tcp://*:7104                                                                                                                    
\.


--
-- Data for Name: config_orchestration; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config_orchestration (rule_id, rule_name, inputs, outputs, processor_id) FROM stdin;
0	rule1                                                           	NISP_PY_CAL_1,NISP_PH_CAL_1,NISP_PJ_CAL_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       	NISP_P_CAL_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    	0
1	rule2                                                           	NISP_P_CAL_1,NISP_PY_1,NISP_PH_1,NISP_PJ_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      	NISP_P_QLA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      	1
2	rule3                                                           	NISP_S_CAL_1,NISP_S_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           	NISP_S_QLA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      	2
3	rule4                                                           	VIS_CAL_1,VIS_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 	VIS_QLA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         	3
4	rule5                                                           	HK_RAW                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          	HK_QLA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          	4
\.


--
-- Data for Name: config_processors; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config_processors (processor_id, processor_name, exe_path, input_path, output_path) FROM stdin;
0	PE_NISP_P_CAL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	/opt/pe/input                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/output                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
1	PE_NISP_P                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       	/opt/pe/bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	/opt/pe/input                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/output                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
2	PE_NISP_S                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       	/opt/pe/bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	/opt/pe/input                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/output                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
3	PE_VIS                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          	/opt/pe/bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	/opt/pe/input                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/output                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
4	PE_HK                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           	/opt/pe/bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	/opt/pe/input                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	/opt/pe/output                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
\.


--
-- Data for Name: config_products; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config_products (product_type) FROM stdin;
NISP_PY_CAL_1                                                                                                                   
NISP_PH_CAL_1                                                                                                                   
NISP_PJ_CAL_1                                                                                                                   
NISP_P_CAL_1                                                                                                                    
NISP_S_CAL_1                                                                                                                    
VIS_CAL_1                                                                                                                       
NISP_PY_1                                                                                                                       
NISP_PH_1                                                                                                                       
NISP_PJ_1                                                                                                                       
NISP_S_1                                                                                                                        
VIS_1                                                                                                                           
NISP_P_QLA                                                                                                                      
NISP_S_QLA                                                                                                                      
VIS_QLA                                                                                                                         
HK_RAW                                                                                                                          
HK_QLA                                                                                                                          
\.


--
-- Data for Name: creators; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY creators (creator_id, creator_desc) FROM stdin;
\.


--
-- Data for Name: instruments; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY instruments (instrument_id, instrument) FROM stdin;
\.


--
-- Data for Name: message_type; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY message_type (message_type, type_desc) FROM stdin;
\.


--
-- Data for Name: messages; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY messages (id, message_id, message_type, origin, destination, message_version, creation_time, transmission_time, reception_time) FROM stdin;
\.


--
-- Name: messages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('messages_id_seq', 1, false);


--
-- Data for Name: observation_modes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY observation_modes (obs_mode, obs_modedesc) FROM stdin;
\.


--
-- Data for Name: product_status; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY product_status (product_status, status_desc) FROM stdin;
\.


--
-- Data for Name: products_info; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY products_info (id, product_id, product_type, product_status, product_version, product_size, creator_id, instrument_id, obs_mode, start_time, end_time, registration_time, url) FROM stdin;
1	EUCL_NISP_PY_CAL_1_20000101T000000-20000101T055959_10                                                                                                                                                                                                           	NISP_PY_CAL_1                                                                                                                   	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-01 00:00:00	2000-01-01 05:59:59	2015-07-15 11:58:59	http://euclid-data.esa.int/data/EUCL_NISP_PY_CAL_1_20000101T000000-20000101T055959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
2	EUCL_NISP_PH_CAL_1_20000101T060000-20000101T115959_10                                                                                                                                                                                                           	NISP_PH_CAL_1                                                                                                                   	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-01 06:00:00	2000-01-01 11:59:59	2015-07-15 11:59:04	http://euclid-data.esa.int/data/EUCL_NISP_PH_CAL_1_20000101T060000-20000101T115959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
3	EUCL_NISP_PJ_CAL_1_20000101T120000-20000101T175959_10                                                                                                                                                                                                           	NISP_PJ_CAL_1                                                                                                                   	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-01 12:00:00	2000-01-01 17:59:59	2015-07-15 11:59:09	http://euclid-data.esa.int/data/EUCL_NISP_PJ_CAL_1_20000101T120000-20000101T175959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
4	EUCL_NISP_P_CAL_1_20000101T180000-20000101T235959_10                                                                                                                                                                                                            	NISP_P_CAL_1                                                                                                                    	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-01 18:00:00	2000-01-01 23:59:59	2015-07-15 11:59:14	http://euclid-data.esa.int/data/EUCL_NISP_P_CAL_1_20000101T180000-20000101T235959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
5	EUCL_NISP_S_CAL_1_20000102T000000-20000102T055959_10                                                                                                                                                                                                            	NISP_S_CAL_1                                                                                                                    	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-02 00:00:00	2000-01-02 05:59:59	2015-07-15 11:59:19	http://euclid-data.esa.int/data/EUCL_NISP_S_CAL_1_20000102T000000-20000102T055959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
6	EUCL_VIS_CAL_1_20000102T060000-20000102T115959_10                                                                                                                                                                                                               	VIS_CAL_1                                                                                                                       	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-02 06:00:00	2000-01-02 11:59:59	2015-07-15 11:59:24	http://euclid-data.esa.int/data/EUCL_VIS_CAL_1_20000102T060000-20000102T115959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
7	EUCL_NISP_PY_1_20000102T120000-20000102T175959_10                                                                                                                                                                                                               	NISP_PY_1                                                                                                                       	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-02 12:00:00	2000-01-02 17:59:59	2015-07-15 11:59:29	http://euclid-data.esa.int/data/EUCL_NISP_PY_1_20000102T120000-20000102T175959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
8	EUCL_NISP_PH_1_20000102T180000-20000102T235959_10                                                                                                                                                                                                               	NISP_PH_1                                                                                                                       	0	1.0                                                                                                                             	12345678	1	1	1	2000-01-02 18:00:00	2000-01-02 23:59:59	2015-07-15 11:59:34	http://euclid-data.esa.int/data/EUCL_NISP_PH_1_20000102T180000-20000102T235959_10.bin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
\.


--
-- Name: products_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('products_info_id_seq', 8, true);


--
-- Data for Name: task_inputs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY task_inputs (task_id, product_id) FROM stdin;
\.


--
-- Data for Name: task_outputs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY task_outputs (task_id, product_id) FROM stdin;
\.


--
-- Data for Name: task_status; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY task_status (task_status, status_desc) FROM stdin;
\.


--
-- Data for Name: tasks_info; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tasks_info (id, task_id, task_status, task_exitcode, task_path, task_size, registration_time, start_time, end_time) FROM stdin;
1	ProcessingTask1                                                                                                                 	0	-1	uno.dos.tres                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    	0	2004-10-19 10:23:54	\N	\N
\.


--
-- Name: tasks_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('tasks_info_id_seq', 1, true);


--
-- Data for Name: variables; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY variables (var_name, var_type, description, smin, smax, hmin, hmax, x) FROM stdin;
\.


--
-- Name: alarms_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY alarms
    ADD CONSTRAINT alarms_pkey PRIMARY KEY (alarm_id);


--
-- Name: config_general_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config_general
    ADD CONSTRAINT config_general_pkey PRIMARY KEY (param_name);


--
-- Name: config_nodes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config_nodes
    ADD CONSTRAINT config_nodes_pkey PRIMARY KEY (node_name);


--
-- Name: config_orchestration_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config_orchestration
    ADD CONSTRAINT config_orchestration_pkey PRIMARY KEY (rule_id);


--
-- Name: config_processors_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config_processors
    ADD CONSTRAINT config_processors_pkey PRIMARY KEY (processor_id);


--
-- Name: config_products_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config_products
    ADD CONSTRAINT config_products_pkey PRIMARY KEY (product_type);


--
-- Name: creators_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY creators
    ADD CONSTRAINT creators_pkey PRIMARY KEY (creator_id);


--
-- Name: instruments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY instruments
    ADD CONSTRAINT instruments_pkey PRIMARY KEY (instrument_id);


--
-- Name: message_type_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY message_type
    ADD CONSTRAINT message_type_pkey PRIMARY KEY (message_type);


--
-- Name: messages_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_pkey PRIMARY KEY (id, message_id, message_type, origin, destination);


--
-- Name: observation_modes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY observation_modes
    ADD CONSTRAINT observation_modes_pkey PRIMARY KEY (obs_mode);


--
-- Name: product_status_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY product_status
    ADD CONSTRAINT product_status_pkey PRIMARY KEY (product_status);


--
-- Name: products_info_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_pkey PRIMARY KEY (id, product_id, product_status, creator_id, instrument_id, obs_mode);


--
-- Name: task_inputs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY task_inputs
    ADD CONSTRAINT task_inputs_pkey PRIMARY KEY (task_id, product_id);


--
-- Name: task_outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY task_outputs
    ADD CONSTRAINT task_outputs_pkey PRIMARY KEY (task_id, product_id);


--
-- Name: task_status_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY task_status
    ADD CONSTRAINT task_status_pkey PRIMARY KEY (task_status);


--
-- Name: tasks_info_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tasks_info
    ADD CONSTRAINT tasks_info_pkey PRIMARY KEY (id, task_id, task_status);


--
-- Name: variables_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY variables
    ADD CONSTRAINT variables_pkey PRIMARY KEY (var_name);


--
-- Name: alarms_var_name_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY alarms
    ADD CONSTRAINT alarms_var_name_fkey FOREIGN KEY (var_name) REFERENCES variables(var_name);


--
-- Name: config_orchestration_processor_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY config_orchestration
    ADD CONSTRAINT config_orchestration_processor_id_fkey FOREIGN KEY (processor_id) REFERENCES config_processors(processor_id);


--
-- Name: products_info_product_type_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_product_type_fkey FOREIGN KEY (product_type) REFERENCES config_products(product_type);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

