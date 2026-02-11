FROM ghcr.io/oran-testing/components_base AS builder

WORKDIR /uuagent

COPY . .

RUN mkdir -p build && rm -rf build/*

WORKDIR /uuagent/build

RUN cmake ../ && \
    make -j$(nproc) && \
    make install

FROM alpine:latest
ENV PYTHONUNBUFFERED=1
RUN apk add --no-cache ca-certificates && update-ca-certificates || true

COPY --from=builder /usr/local /usr/local

ENV ARGS=""
CMD [ "sh", "-c", "/usr/local/bin/uuagent -c /uu-agent.conf ${ARGS}" ]

#CMD [ "sh", "-c", "/usr/local/bin/uuagent /uu-agent.conf" ]
