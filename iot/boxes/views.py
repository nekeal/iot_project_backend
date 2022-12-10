import json
from typing import Any, Dict

from django.contrib import messages
from django.contrib.admin import site as admin_site
from django.http import HttpRequest, HttpResponse
from django.shortcuts import redirect
from django.views.generic import FormView, ListView, TemplateView
from rest_framework.generics import CreateAPIView, get_object_or_404

from iot.accounts.models import CustomUser
from iot.boxes.client import BoxMqttClient
from iot.boxes.forms import PublishMessageForm
from iot.boxes.models import Organizer, TimeOfDay


class PublishMessageFormView(FormView):
    form_class = PublishMessageForm
    template_name = "publish_message.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        return {
            **context,
            **admin_site.each_context(self.request),
            "media": self.form_class().media,
        }

    def form_valid(self, form):
        messages.success(self.request, "Successfully published message")
        return self.get(self.request)

    def post(self, request: HttpRequest, *args: Any, **kwargs: Any) -> HttpResponse:
        form = self.get_form()
        if form.is_valid():
            BoxMqttClient().publish(
                form.cleaned_data["topic"], json.dumps(form.cleaned_data["message"])
            )
            return self.form_valid(form)
        else:
            return self.form_invalid(form)


class BoxConfigView(TemplateView):
    template_name = "konfiguracja.html"

    def get_context_data(self, **kwargs: Any) -> Dict[str, Any]:
        return {
            **super().get_context_data(**kwargs),
            "organizers": Organizer.objects.order_by("id"),
            "times_of_day": TimeOfDay.objects.all(),
        }


class TimeOfDayView(ListView):
    template_name = "pory.html"
    queryset = TimeOfDay.objects.all()


class AddOrganizerView(TemplateView):
    template_name = "dodaj_organizer.html"


class MainPageView(TemplateView):
    template_name = "glowna.html"


class ProfileView(TemplateView):
    template_name = "profil.html"


class WelcomePageView(TemplateView):
    template_name = "welcome_page.html"


class ShareProfileView(CreateAPIView):
    def post(self, request, *args, **kwargs):
        username = request.data.get("username")
        user = get_object_or_404(CustomUser, username=username)
        for organizer in Organizer.objects.filter(owner=request.user):
            organizer.users.add(user)
        messages.success(
            request, f"Profil został udostępniony dla użytkownika {username}"
        )
        return redirect("boxes:profile")


class RegisterOrganizerView(CreateAPIView):
    def post(self, request, *args, **kwargs):
        Organizer.objects.create(
            name=request.data.get("name"),
            serial_number=request.data.get("serial_number"),
            owner=request.user,
        )
        messages.success(
            request, f"Organizer '{request.data.get('name')}' został zarejestrowany"
        )
        return redirect("boxes:box_config")
