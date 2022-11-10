import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault(
    "DJANGO_SETTINGS_MODULE", "iot_project_backend.settings.production"
)

application = get_wsgi_application()
