# IoT project 

[![CI](https://github.com/nekeal/iot_project_backend/actions/workflows/backend.yml/badge.svg)](https://github.com/nekeal/iot_project_backend/actions)

Grupa A - projekt na przedmiot IoT semestr zimowy 2022/23

# Prerequisites

## Native way with virtualenv
- [Python3.10](https://www.python.org/downloads/)
- [Virtualenv](https://virtualenv.pypa.io/en/latest/)

## Docker way
- [Docker](https://docs.docker.com/engine/install/)  
- [Docker Compose](https://docs.docker.com/compose/install/)

## Local Development

## Native way with virtualenv

First create postgresql database:

```sql
create user iot_project_backend with createdb;
alter user iot_project_backend password 'iot_project_backend';
create database iot_project_backend owner iot_project_backend;
```
Now you can setup virtualenv and django:
```bash
virtualenv venv
source venv/bin/activate
pip install pip-tools
make bootstrap
```

## Docker way

Start the dev server for local development:
```bash
docker-compose up
```

Run a command inside the docker container:

```bash
docker-compose run --rm web [command]
```
