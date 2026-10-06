FROM node:22-alpine

# gcc/headers for student code; g++/python3/make to build node-pty;
# tini reaps orphaned processes (PID 1 duty); procps for pkill/pgrep/ps.
RUN apk add --no-cache \
    gcc g++ musl-dev linux-headers make python3 \
    bash coreutils less nano \
    tini procps

WORKDIR /app
COPY package.json ./
RUN npm install --omit=dev

COPY server.js ./
COPY sandbox/sandbox-launch.c ./sandbox/
COPY cs-bashrc /etc/cs-bashrc

# The launcher is root-only; students get their own copy to read in Lesson 16.
RUN mkdir -p /usr/local/sbin \
 && gcc -Wall -Wextra -Werror -O2 -o /usr/local/sbin/sandbox-launch sandbox/sandbox-launch.c \
 && chmod 0700 /usr/local/sbin/sandbox-launch \
 && addgroup -S students \
 && mkdir -p /work && chmod 0711 /work \
 && chmod -R go-w /app /etc/cs-bashrc

EXPOSE 8080
ENTRYPOINT ["/sbin/tini", "--"]
CMD ["node", "server.js"]
