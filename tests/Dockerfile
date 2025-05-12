# Base image
FROM ubuntu

# Prevent interactive prompts during install
ENV DEBIAN_FRONTEND=noninteractive

# Tools and libraries
RUN apt update && apt install -y \
    build-essential \
    valgrind \
    libunistring-dev \
    curl \
    git \
    ca-certificates \
    locales \
    && locale-gen en_US.UTF-8

# UTF-8 locale
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

# Custom prompt
COPY prompt.sh /volume/
RUN echo ". /volume/prompt.sh" >> /root/.bashrc \
 && echo ". /volume/prompt.sh" >> /etc/skel/.bashrc

# Working directory
WORKDIR /volume

# Default shell
CMD ["/bin/bash"]
