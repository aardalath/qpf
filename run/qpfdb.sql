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
-- Name: alarms; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE alarms (
    alarm_id integer NOT NULL,
    var_name character varying(50),
    origin character varying(200),
    severity integer,
    status integer,
    curr_severity integer,
    curr_status integer,
    creation timestamp without time zone,
    acknowledged timestamp without time zone,
    last_update timestamp without time zone,
    alarm_state integer
);


ALTER TABLE alarms OWNER TO eucops;

--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE; Schema: public; Owner: eucops
--

CREATE SEQUENCE alarms_alarm_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE alarms_alarm_id_seq OWNER TO eucops;

--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eucops
--

ALTER SEQUENCE alarms_alarm_id_seq OWNED BY alarms.alarm_id;


--
-- Name: configuration; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE configuration (
    created timestamp without time zone NOT NULL,
    cfg json,
    last_accessed timestamp without time zone
);


ALTER TABLE configuration OWNER TO eucops;

--
-- Name: creators; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE creators (
    creator_id integer NOT NULL,
    creator_desc character varying(128)
);


ALTER TABLE creators OWNER TO eucops;

--
-- Name: instruments; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE instruments (
    instrument_id integer NOT NULL,
    instrument character varying(128)
);


ALTER TABLE instruments OWNER TO eucops;

--
-- Name: message_type; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE message_type (
    message_type integer NOT NULL,
    type_desc character varying(128)
);


ALTER TABLE message_type OWNER TO eucops;

--
-- Name: messages; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE messages (
    id integer NOT NULL,
    message_id character varying(128) NOT NULL,
    message_type character varying(128) NOT NULL,
    origin character varying(128) NOT NULL,
    destination character varying(128) NOT NULL,
    message_version integer,
    creation_time timestamp without time zone,
    transmission_time timestamp without time zone,
    reception_time timestamp without time zone
);


ALTER TABLE messages OWNER TO eucops;

--
-- Name: messages_id_seq; Type: SEQUENCE; Schema: public; Owner: eucops
--

CREATE SEQUENCE messages_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE messages_id_seq OWNER TO eucops;

--
-- Name: messages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eucops
--

ALTER SEQUENCE messages_id_seq OWNED BY messages.id;


--
-- Name: node_states; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE node_states (
    node_name character varying(20) NOT NULL,
    node_state character varying(32)
);


ALTER TABLE node_states OWNER TO eucops;

--
-- Name: observation_modes; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE observation_modes (
    obsmode_id integer NOT NULL,
    obsmode_desc character varying(128)
);


ALTER TABLE observation_modes OWNER TO eucops;

--
-- Name: product_status; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE product_status (
    product_status_id integer NOT NULL,
    status_desc character varying(128)
);


ALTER TABLE product_status OWNER TO eucops;

--
-- Name: products_info; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE products_info (
    id integer NOT NULL,
    product_id character varying(256) NOT NULL,
    product_type character varying(128),
    product_version character varying(128),
    product_size bigint,
    product_status_id integer,
    creator_id integer,
    instrument_id integer,
    start_time timestamp without time zone,
    end_time timestamp without time zone,
    registration_time timestamp without time zone,
    url character varying(1024),
    signature character varying,
    obsmode_id integer NOT NULL
);


ALTER TABLE products_info OWNER TO eucops;

--
-- Name: products_info_id_seq; Type: SEQUENCE; Schema: public; Owner: eucops
--

CREATE SEQUENCE products_info_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE products_info_id_seq OWNER TO eucops;

--
-- Name: products_info_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eucops
--

ALTER SEQUENCE products_info_id_seq OWNED BY products_info.id;


--
-- Name: task_inputs; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE task_inputs (
    task_id character varying(128) NOT NULL,
    product_id character varying(256) NOT NULL
);


ALTER TABLE task_inputs OWNER TO eucops;

--
-- Name: task_outputs; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE task_outputs (
    task_id character varying(128) NOT NULL,
    product_id character varying(256) NOT NULL
);


ALTER TABLE task_outputs OWNER TO eucops;

--
-- Name: task_status; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE task_status (
    task_status_id integer NOT NULL,
    status_desc character varying(128)
);


ALTER TABLE task_status OWNER TO eucops;

--
-- Name: tasks_info; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE tasks_info (
    id integer NOT NULL,
    task_id character varying(128) NOT NULL,
    task_status_id integer,
    task_exitcode integer,
    task_path character varying(1024),
    task_size integer,
    registration_time timestamp without time zone,
    start_time timestamp without time zone,
    end_time timestamp without time zone,
    task_data json
);


ALTER TABLE tasks_info OWNER TO eucops;

--
-- Name: tasks_info_id_seq; Type: SEQUENCE; Schema: public; Owner: eucops
--

CREATE SEQUENCE tasks_info_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE tasks_info_id_seq OWNER TO eucops;

--
-- Name: tasks_info_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eucops
--

ALTER SEQUENCE tasks_info_id_seq OWNED BY tasks_info.id;


--
-- Name: transmissions; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE transmissions (
    id integer NOT NULL,
    msg_date timestamp without time zone,
    msg_from character(64),
    msg_to character(64),
    msg_type character(64),
    msg_bcast character(1),
    msg_content text
);


ALTER TABLE transmissions OWNER TO eucops;

--
-- Name: transmissions_id_seq; Type: SEQUENCE; Schema: public; Owner: eucops
--

CREATE SEQUENCE transmissions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE transmissions_id_seq OWNER TO eucops;

--
-- Name: transmissions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eucops
--

ALTER SEQUENCE transmissions_id_seq OWNED BY transmissions.id;


--
-- Name: variables; Type: TABLE; Schema: public; Owner: eucops; Tablespace:
--

CREATE TABLE variables (
    var_name character varying(50) NOT NULL,
    var_type integer,
    description character varying(200),
    smin numeric,
    smax numeric,
    hmin numeric,
    hmax numeric,
    x numeric
);


ALTER TABLE variables OWNER TO eucops;

--
-- Name: alarm_id; Type: DEFAULT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY alarms ALTER COLUMN alarm_id SET DEFAULT nextval('alarms_alarm_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY messages ALTER COLUMN id SET DEFAULT nextval('messages_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY products_info ALTER COLUMN id SET DEFAULT nextval('products_info_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY tasks_info ALTER COLUMN id SET DEFAULT nextval('tasks_info_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY transmissions ALTER COLUMN id SET DEFAULT nextval('transmissions_id_seq'::regclass);


--
-- Data for Name: alarms; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY alarms (alarm_id, var_name, origin, severity, status, curr_severity, curr_status, creation, acknowledged, last_update, alarm_state) FROM stdin;
\.


--
-- Name: alarms_alarm_id_seq; Type: SEQUENCE SET; Schema: public; Owner: eucops
--

SELECT pg_catalog.setval('alarms_alarm_id_seq', 1, false);


--
-- Data for Name: configuration; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY configuration (created, cfg, last_accessed) FROM stdin;
2016-02-09 14:01:42	{"connections":{"DataMng":["EvtMng","TskMng","TskOrc"],"EvtMng":["QPFHMI","DataMng","LogMng","TskMng","TskOrc","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5"],"LogMng":["EvtMng","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5"],"QPFHMI":["EvtMng"],"TskAge1":["EvtMng","LogMng","TskMng"],"TskAge2":["EvtMng","LogMng","TskMng"],"TskAge3":["EvtMng","LogMng","TskMng"],"TskAge4":["EvtMng","LogMng","TskMng"],"TskAge5":["EvtMng","LogMng","TskMng"],"TskMng":["EvtMng","TskOrc","DataMng","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5"],"TskOrc":["EvtMng","TskMng","DataMng"]},"db":{"host":"127.0.0.1","name":"qpfdb","port":"5432","pwd":"euclidjcg","user":"jcgonzalez"},"general":{"app_name":"QPF","app_version":"0.1","last_access":"20150616T121555","run_path":"/var/run/qpf"},"machines":{"eucl06":["QPFHMI","EvtMng","DataMng","LogMng","TskOrc","TskMng"],"vmu1":["TskAge1","TskAge2","TskAge3"],"vmu2":["TskAge4","TskAge5"]},"nodes":{"hmi_node":"QPFHMI","node_list":{"DataMng":{"client":"tcp://192.168.56.1:7101","server":"tcp://*:7101","type":"datamng"},"EvtMng":{"client":"tcp://192.168.56.1:7100","server":"tcp://*:7100","type":"evtmng"},"LogMng":{"client":"tcp://192.168.56.1:7102","server":"tcp://*:7102","type":"logmng"},"QPFHMI":{"client":"tcp://192.168.56.1:7999","server":"tcp://*:7999","type":"hmi"},"TskAge1":{"client":"tcp://192.168.56.101:7111","server":"tcp://*:7111","type":"taskagent"},"TskAge2":{"client":"tcp://192.168.56.101:7112","server":"tcp://*:7112","type":"taskagent"},"TskAge3":{"client":"tcp://192.168.56.101:7113","server":"tcp://*:7113","type":"taskagent"},"TskAge4":{"client":"tcp://192.168.56.102:7114","server":"tcp://*:7114","type":"taskagent"},"TskAge5":{"client":"tcp://192.168.56.102:7115","server":"tcp://*:7115","type":"taskagent"},"TskMng":{"client":"tcp://192.168.56.1:7104","server":"tcp://*:7104","type":"taskmng"},"TskOrc":{"client":"tcp://192.168.56.1:7103","server":"tcp://*:7103","type":"taskorc"}}},"orchestration":{"rules":{"VIS_LE1_Ingestion":{"condition":"(VIS_LE1.date==VIS_LE1_META.date) & (VIS_LE1.time==VIS_LE1_META.time)","inputs":"VIS_LE1,VIS_LE1_META","outputs":"VIS_LE1_INGLOG","processing":"LE1_VIS_Ingestor"},"VIS_LE1_MetadataCollection":{"condition":"1","inputs":"VIS_INFO","outputs":"VIS_LE1_META","processing":"LE1_VIS_MetadataCollector"},"VIS_LE1_Processing":{"condition":"1","inputs":"VIS_RAW","outputs":"VIS_LE1,VIS_LE1_LOG","processing":"LE1_VIS_Processor"}}},"processing":{"processors":{"LE1_VIS_Ingestor":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_Ingestor","output_path":"/opt/pe/output"},"LE1_VIS_MetadataCollector":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_MetadataCollector","output_path":"/opt/pe/output"},"LE1_VIS_Processor":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_Processor","output_path":"/opt/pe/output"}}},"products":{"data_ext":"fits","log_ext":"log","meta_ext":"xml","parsing_assign":"%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5","parsing_regex":"@filenamespec.tpl","product_datatypes":null,"product_id_tpl":"%M_%T_%S_%f_%v","product_types":["VIS_RAW","VIS_INFO","VIS_LE1","VIS_LE1_LOG","VIS_LE1_META","VIS_LE1_INGLOG","NIR_RAW","NIR_INFO","NIR_LE1","NIR_LE1_LOG","NIR_LE1_META","NIR_LE1_INGLOG","SIR_RAW","SIR_INFO","SIR_LE1","SIR_LE1_LOG","SIR_LE1_META","SIR_LE1_INGLOG","HK_RAW","HK_QLA","UNKNOWN_TYPE"]},"storage":{"base":{"path":"/qpf/run"},"in":{"address":"0.0.0.0","inbox":"/qpf/data/inbox","password":"","port":"0","protocol":"RLINK","user":""},"local":{"path":"/qpf/data/archive"},"out":{"address":"127.0.0.1","password":"@","port":"2121","protocol":"FTP","user":"anonymous"},"shared":{"external_path":"/media/jcgonzalez/shared/QPF_shared/data","local_path":"/home/jcgonzalez/shared/QPF_shared/data"}}}\n	2016-02-09 14:01:42
2016-04-01 09:03:46	{"connections":{"DataMng":["EvtMng","TskMng","TskOrc"],"EvtMng":["QPFHMI","DataMng","LogMng","TskMng","TskOrc","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5","TskAge6","TskAge7"],"LogMng":["EvtMng","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5","TskAge6","TskAge7"],"QPFHMI":["EvtMng"],"TskAge1":["EvtMng","LogMng","TskMng"],"TskAge2":["EvtMng","LogMng","TskMng"],"TskAge3":["EvtMng","LogMng","TskMng"],"TskAge4":["EvtMng","LogMng","TskMng"],"TskAge5":["EvtMng","LogMng","TskMng"],"TskAge6":["EvtMng","LogMng","TskMng"],"TskAge7":["EvtMng","LogMng","TskMng"],"TskMng":["EvtMng","TskOrc","DataMng","TskAge1","TskAge2","TskAge3","TskAge4","TskAge5","TskAge6","TskAge7"],"TskOrc":["EvtMng","TskMng","DataMng"]},"db":{"host":"127.0.0.1","name":"qpfdb","port":"5432","pwd":"","user":"eucops"},"general":{"app_name":"QPF","app_version":"0.1","last_access":"20150616T121555","run_path":"/home/eucops/qpf/run"},"machines":{"eucdev02.net3.lan":["QPFHMI","EvtMng","DataMng","LogMng","TskOrc","TskMng","TskAge1","TskAge2"],"eucdev03.net3.lan":["TskAge3","TskAge4","TskAge5","TskAge6","TskAge7"]},"nodes":{"hmi_node":"QPFHMI","master_machine":"eucdev02.net3.lan","node_list":{"DataMng":{"client":"tcp://10.66.180.97:7101","server":"tcp://*:7101","type":"datamng"},"EvtMng":{"client":"tcp://10.66.180.97:7100","server":"tcp://*:7100","type":"evtmng"},"LogMng":{"client":"tcp://10.66.180.97:7102","server":"tcp://*:7102","type":"logmng"},"QPFHMI":{"client":"tcp://10.66.180.97:7999","server":"tcp://*:7999","type":"hmi"},"TskAge1":{"client":"tcp://10.66.180.97:7111","server":"tcp://*:7111","type":"taskagent"},"TskAge2":{"client":"tcp://10.66.180.97:7112","server":"tcp://*:7112","type":"taskagent"},"TskAge3":{"client":"tcp://10.66.180.95:7113","server":"tcp://*:7113","type":"taskagent"},"TskAge4":{"client":"tcp://10.66.180.95:7114","server":"tcp://*:7114","type":"taskagent"},"TskAge5":{"client":"tcp://10.66.180.95:7115","server":"tcp://*:7115","type":"taskagent"},"TskAge6":{"client":"tcp://10.66.180.95:7116","server":"tcp://*:7116","type":"taskagent"},"TskAge7":{"client":"tcp://10.66.180.95:7117","server":"tcp://*:7117","type":"taskagent"},"TskMng":{"client":"tcp://10.66.180.97:7104","server":"tcp://*:7104","type":"taskmng"},"TskOrc":{"client":"tcp://10.66.180.97:7103","server":"tcp://*:7103","type":"taskorc"}}},"orchestration":{"rules":{"VIS_LE1_Ingestion":{"condition":"(VIS_LE1.date==VIS_LE1_META.date) & (VIS_LE1.time==VIS_LE1_META.time)","inputs":"VIS_LE1,VIS_LE1_META","outputs":"VIS_LE1_INGLOG","processing":"LE1_VIS_Ingestor"},"VIS_LE1_MetadataCollection":{"condition":"1","inputs":"VIS_INFO","outputs":"VIS_LE1_META","processing":"LE1_VIS_MetadataCollector"},"VIS_LE1_Processing":{"condition":"1","inputs":"VIS_RAW","outputs":"VIS_LE1,VIS_LE1_LOG","processing":"LE1_VIS_Processor"}}},"processing":{"processors":{"LE1_VIS_Ingestor":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_Ingestor","output_path":"/opt/pe/output"},"LE1_VIS_MetadataCollector":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_MetadataCollector","output_path":"/opt/pe/output"},"LE1_VIS_Processor":{"exe_path":"/opt/pe/bin","input_path":"/opt/pe/input","name":"LE1_VIS_Processor","output_path":"/opt/pe/output"}}},"products":{"data_ext":"fits","log_ext":"log","meta_ext":"xml","parsing_assign":"%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5","parsing_regex":"@filenamespec.tpl","product_datatypes":null,"product_id_tpl":"%M_%T_%S_%f","product_types":["VIS_RAW","VIS_INFO","VIS_LE1","VIS_LE1_LOG","VIS_LE1_META","VIS_LE1_INGLOG","NIR_RAW","NIR_INFO","NIR_LE1","NIR_LE1_LOG","NIR_LE1_META","NIR_LE1_INGLOG","SIR_RAW","SIR_INFO","SIR_LE1","SIR_LE1_LOG","SIR_LE1_META","SIR_LE1_INGLOG","HK_RAW","HK_QLA","UNKNOWN_TYPE"]},"storage":{"archive":{"address":"eucdev.n1data.lan","password":"","path":"/home/eucops/ws/jcgg/DSS_EAS_Proxy/incoming","port":"","protocol":"SCP","user":"eucops"},"base":{"path":"/home/eucops/qpf/run"},"gateway":{"path":"/home/eucops/qpf/data/gateway"},"incoming":{"address":"0.0.0.0","password":"yyyyy","path":"/home/eucops/qpf/data/inbox","port":"0","protocol":"NOTUSED","user":"xxxxx"},"local_archive":{"path":"/home/eucops/qpf/data/archive"},"outgoing":{"address":"0.0.0.0","password":"yyyyy","path":"/home/eucops/qpf/data/outbox","port":"0","protocol":"NOTUSED","user":"xxxxx"}}}\n	2016-04-01 09:03:46
\.


--
-- Data for Name: creators; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY creators (creator_id, creator_desc) FROM stdin;
1	SOC_L1P
10	SOC_QLA.TEST
11	SOC_QLA.OPE
\.


--
-- Data for Name: instruments; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY instruments (instrument_id, instrument) FROM stdin;
1	VIS
2	NISP.P
3	NISP.S
-1	UNKNOWN_INST
\.


--
-- Data for Name: message_type; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY message_type (message_type, type_desc) FROM stdin;
\.


--
-- Data for Name: messages; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY messages (id, message_id, message_type, origin, destination, message_version, creation_time, transmission_time, reception_time) FROM stdin;
\.


--
-- Name: messages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: eucops
--

SELECT pg_catalog.setval('messages_id_seq', 1, false);


--
-- Data for Name: node_states; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY node_states (node_name, node_state) FROM stdin;
\.


--
-- Data for Name: observation_modes; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY observation_modes (obsmode_id, obsmode_desc) FROM stdin;
1	NOMINAL
2	TEST
\.


--
-- Data for Name: product_status; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY product_status (product_status_id, status_desc) FROM stdin;
0	OK
1	NOTOK
\.


--
-- Data for Name: products_info; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY products_info (id, product_id, product_type, product_version, product_size, product_status_id, creator_id, instrument_id, start_time, end_time, registration_time, url, signature, obsmode_id) FROM stdin;
\.


--
-- Name: products_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: eucops
--

SELECT pg_catalog.setval('products_info_id_seq', 69106, true);


--
-- Data for Name: task_inputs; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY task_inputs (task_id, product_id) FROM stdin;
\.


--
-- Data for Name: task_outputs; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY task_outputs (task_id, product_id) FROM stdin;
\.


--
-- Data for Name: task_status; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY task_status (task_status_id, status_desc) FROM stdin;
-1	FAILED
0	FINISHED
1	RUNNING
2	WAITING
-2	SCHEDULED
\.


--
-- Data for Name: tasks_info; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY tasks_info (id, task_id, task_status_id, task_exitcode, task_path, task_size, registration_time, start_time, end_time, task_data) FROM stdin;
\.


--
-- Name: tasks_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: eucops
--

SELECT pg_catalog.setval('tasks_info_id_seq', 22606, true);


--
-- Data for Name: transmissions; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY transmissions (id, msg_date, msg_from, msg_to, msg_type, msg_bcast, msg_content) FROM stdin;
\.


--
-- Name: transmissions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: eucops
--

SELECT pg_catalog.setval('transmissions_id_seq', 928641, true);


--
-- Data for Name: variables; Type: TABLE DATA; Schema: public; Owner: eucops
--

COPY variables (var_name, var_type, description, smin, smax, hmin, hmax, x) FROM stdin;
\.


--
-- Name: alarms_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY alarms
    ADD CONSTRAINT alarms_pkey PRIMARY KEY (alarm_id);


--
-- Name: configuration_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY configuration
    ADD CONSTRAINT configuration_pkey PRIMARY KEY (created);


--
-- Name: creators_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY creators
    ADD CONSTRAINT creators_pkey PRIMARY KEY (creator_id);


--
-- Name: instruments_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY instruments
    ADD CONSTRAINT instruments_pkey PRIMARY KEY (instrument_id);


--
-- Name: message_type_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY message_type
    ADD CONSTRAINT message_type_pkey PRIMARY KEY (message_type);


--
-- Name: messages_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_pkey PRIMARY KEY (id, message_id, message_type, origin, destination);


--
-- Name: node_states_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY node_states
    ADD CONSTRAINT node_states_pkey PRIMARY KEY (node_name);


--
-- Name: observation_modes_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY observation_modes
    ADD CONSTRAINT observation_modes_pkey PRIMARY KEY (obsmode_id);


--
-- Name: product_status_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY product_status
    ADD CONSTRAINT product_status_pkey PRIMARY KEY (product_status_id);


--
-- Name: products_info_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_pkey PRIMARY KEY (product_id);


--
-- Name: task_inputs_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY task_inputs
    ADD CONSTRAINT task_inputs_pkey PRIMARY KEY (task_id, product_id);


--
-- Name: task_outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY task_outputs
    ADD CONSTRAINT task_outputs_pkey PRIMARY KEY (task_id, product_id);


--
-- Name: task_status_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY task_status
    ADD CONSTRAINT task_status_pkey PRIMARY KEY (task_status_id);


--
-- Name: tasks_info_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY tasks_info
    ADD CONSTRAINT tasks_info_pkey PRIMARY KEY (task_id);


--
-- Name: transmissions_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY transmissions
    ADD CONSTRAINT transmissions_pkey PRIMARY KEY (id);


--
-- Name: variables_pkey; Type: CONSTRAINT; Schema: public; Owner: eucops; Tablespace:
--

ALTER TABLE ONLY variables
    ADD CONSTRAINT variables_pkey PRIMARY KEY (var_name);


--
-- Name: alarms_var_name_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY alarms
    ADD CONSTRAINT alarms_var_name_fkey FOREIGN KEY (var_name) REFERENCES variables(var_name);


--
-- Name: products_info_creator_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_creator_id_fkey FOREIGN KEY (creator_id) REFERENCES creators(creator_id);


--
-- Name: products_info_instrument_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_instrument_id_fkey FOREIGN KEY (instrument_id) REFERENCES instruments(instrument_id);


--
-- Name: products_info_product_status_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY products_info
    ADD CONSTRAINT products_info_product_status_id_fkey FOREIGN KEY (product_status_id) REFERENCES product_status(product_status_id);


--
-- Name: task_inputs_task_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY task_inputs
    ADD CONSTRAINT task_inputs_task_id_fkey FOREIGN KEY (task_id) REFERENCES tasks_info(task_id);


--
-- Name: task_outputs_task_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY task_outputs
    ADD CONSTRAINT task_outputs_task_id_fkey FOREIGN KEY (task_id) REFERENCES tasks_info(task_id);


--
-- Name: tasks_info_task_status_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eucops
--

ALTER TABLE ONLY tasks_info
    ADD CONSTRAINT tasks_info_task_status_id_fkey FOREIGN KEY (task_status_id) REFERENCES task_status(task_status_id);


--
-- Name: public; Type: ACL; Schema: -; Owner: eucops
--

GRANT ALL ON SCHEMA public TO PUBLIC;
GRANT ALL ON SCHEMA public TO eucops;


--
-- PostgreSQL database dump complete
--
