FROM debian
RUN apt-get update && apt-get install -y curl build-essential
COPY . /opt/ipspoof/
WORKDIR /opt/ipspoof/
RUN make
ENTRYPOINT ["./dist/ipspoof"]
