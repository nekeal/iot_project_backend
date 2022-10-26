from .base import *

SECRET_KEY = "secret_key"

# ------------- DATABASES -------------
DATABASES = {
    "default": {
        "ENGINE": "django.db.backends.postgresql",
        "NAME": env("POSTGRES_DB", "iot_project_backend"),
        "USER": env("POSTGRES_USER", "iot_project_backend"),
        "PASSWORD": env("POSTGRES_PASSWORD", "iot_project_backend"),
        "HOST": env("POSTGRES_HOST", "localhost"),
    }
}
