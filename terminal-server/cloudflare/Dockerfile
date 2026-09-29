FROM node:22-alpine

RUN apk add --no-cache \
    gcc \
    g++ \
    musl-dev \
    linux-headers \
    make \
    bash \
    coreutils \
    less \
    nano \
    python3

WORKDIR /app

COPY package.json ./
RUN npm install --production

COPY server.js ./
COPY bashrc /etc/skel/.bashrc

RUN adduser -D -s /bin/bash student \
    && cp /etc/skel/.bashrc /home/student/.bashrc \
    && chown student:student /home/student/.bashrc

EXPOSE 8080

CMD ["node", "server.js"]
