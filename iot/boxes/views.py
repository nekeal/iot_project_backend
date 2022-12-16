import json
from typing import Any, Dict

from django.contrib import messages
from django.contrib.admin import site as admin_site
from django.contrib.auth.mixins import LoginRequiredMixin
from django.contrib.auth.views import LoginView
from django.http import HttpRequest, HttpResponse
from django.shortcuts import redirect
from django.views.generic import FormView, ListView, TemplateView
from rest_framework.generics import CreateAPIView, get_object_or_404

from iot.accounts.models import CustomUser
from iot.boxes.client import BoxMqttClient
from iot.boxes.forms import PublishMessageForm
from iot.boxes.message_parser import BoxConfiguration
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


class BoxConfigView(LoginRequiredMixin, TemplateView):
    template_name = "konfiguracja.html"

    def get_context_data(self, **kwargs: Any) -> Dict[str, Any]:
        return {
            **super().get_context_data(**kwargs),
            "organizers": Organizer.objects.order_by("id"),
            "times_of_day": TimeOfDay.objects.all(),
        }

    def post(self, request, *args, **kwargs):
        url = str(request.path)
        box_id = url.split("/")[-2]
        column_id = url.split("/")[-1]
        med = BoxConfiguration(
            name=request.POST.get("medicineName", ""),
            column=column_id,
            times=request.POST.getlist("times", []),
            days=request.POST.getlist("days", []),
            sound=request.POST.get("sound", ""),
            light=request.POST.get("light", ""),
        )
        configuration_dict = med.generate_configuration()
        Organizer.objects.filter(id=int(box_id)).update(**{f"column_{column_id}": configuration_dict})
        topic_name = f"update/{box_id}"
        BoxMqttClient().publish(topic_name, json.dumps(configuration_dict))

        messages.success(
            request, f"Konfiguracja kolumny '{column_id}' została zarejestrowana"
        )
        return redirect("boxes:box_config")


class TimeOfDayView(LoginRequiredMixin, ListView):
    template_name = "pory.html"
    queryset = TimeOfDay.objects.all()


class AddOrganizerView(LoginRequiredMixin, TemplateView):
    template_name = "dodaj_organizer.html"


class MainPageView(LoginRequiredMixin, TemplateView):
    template_name = "glowna.html"


class ProfileView(LoginRequiredMixin, TemplateView):
    template_name = "profil.html"


class WelcomePageView(TemplateView):
    template_name = "welcome_page.html"

    def get(self, request: HttpRequest, *args: Any, **kwargs: Any) -> HttpResponse:
        if request.user.is_authenticated:
            return redirect("boxes:box_config")
        return super().get(request, *args, **kwargs)

    def post(self, request: HttpRequest, *args: Any, **kwargs: Any):
        response = LoginView.as_view(
            template_name="welcome_page.html", next_page="boxes:box_config"
        )(request)
        return response


class ShareProfileView(LoginRequiredMixin, CreateAPIView):
    def post(self, request, *args, **kwargs):
        username = request.data.get("username")
        user = get_object_or_404(CustomUser, username=username)
        for organizer in Organizer.objects.filter(owner=request.user):
            organizer.users.add(user)
        messages.success(
            request, f"Profil został udostępniony dla użytkownika {username}"
        )
        return redirect("boxes:profile")


class RegisterOrganizerView(LoginRequiredMixin, CreateAPIView):
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
