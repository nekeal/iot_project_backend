from .base import *

SECRET_KEY = "secret_key"

INSTALLED_APPS.append("debug_toolbar")

MIDDLEWARE.insert(0, "debug_toolbar.middleware.DebugToolbarMiddleware")

INTERNAL_IPS = [
    "127.0.0.1",
]

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
