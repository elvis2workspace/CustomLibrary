FROM markadams/chromium-xvfb-py2
WORKDIR /opt/zs5s/download-mail
COPY ./download-mail/.pip /root/.pip/
COPY ./download-mail/requirements.txt /opt/zs5s/download-mail/requirements.txt

RUN pip install --upgrade pip

RUN pip install -r requirements.txt && mkdir /tmp/downloaded_files

ENV DISPLAY :1
COPY ./download-mail /opt/zs5s/download-mail
COPY ./data_service /opt/zs5s/data_service
COPY ./save_mail /opt/zs5s/save_mail
COPY ./common/ /opt/zs5s/common
USER root

CMD sh start.sh