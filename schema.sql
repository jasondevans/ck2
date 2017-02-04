pragma key='test';

drop table if exists sites;
create table sites (
id integer primary key autoincrement,
site_id integer,
name text, 
url text, 
user text, 
password text,
notes text,
version integer, 
deleted integer default 0);

insert into sites (site_id, name, url, user, password, notes, version)
values (1, 'Netflix', 'http://netflix.com', 'netflix_user',
'mypassw0rd', 'My netflix notes.', 1);

insert into sites (site_id, name, url, user, password, notes, version)
values (2, 'Amazon', 'http://amazon.com', 'amazon_user',
'mypassw0rd', 'My amazon notes.', 1);

drop table if exists metadata;
create table metadata (
id integer primary key,
guid text, 
friendly_name text, 
last_modified_utc text);

insert into metadata (id, guid, friendly_name, last_modified_utc)
values (
1,
'df4e1ce2-a6a9-4c00-85b8-adcc864cabe', 
'My Password Manager', 
'2015/07/11 16:07:23 334');
