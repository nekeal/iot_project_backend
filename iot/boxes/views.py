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
from iot.boxes.forms import PublishMessageForm, GetMessageForm
from iot.boxes.models import Organizer, TimeOfDay, TopicMessages


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


class GetMessageFormView(FormView):
    form_class = GetMessageForm

    template_name = "listen_message.html"
    query_result = []

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        return {
            **context,
            **admin_site.each_context(self.request),
            "media": self.form_class().media,
            "response": [ "date: {}, message: {}".format(result.fetch_date, result.message) for result in self.query_result ],
        }

    def form_valid(self, form):
        messages.success(self.request, "Data loaded sucesfully")
        return self.get(self.request)

    def post(self, request: HttpRequest, *args: Any, **kwargs: Any) -> HttpResponse:
        form = self.get_form()
        if form.is_valid():
            self.query_result = TopicMessages.objects.filter(topic=form.cleaned_data["topic"]).order_by("-fetch_date")
            return self.form_valid(form)
        else:
            return self.form_invalid(form)
