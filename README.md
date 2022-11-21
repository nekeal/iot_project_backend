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

We use [Docker Buildkit](https://docs.docker.com/develop/develop-images/build_enhancements/)
so make sure you have it enabled.

Start the dev server for local development:

```bash
make bootstrap-docker
```
You can use the above command later on to ensure that you have built newest
image and all migrations are applied to the database.

Usually you want to use `docker-compose up` command to start containers.
Web interface is exposed unset `8000` port. 

Then you can run any command inside the docker container:

```bash
docker-compose run --rm web [command]
```

## Default login credentials

login: admin

password: administrator
