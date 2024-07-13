# Dockerfile for a C application without multi-stage build
FROM gcc:latest
RUN apt-get update && apt-get install -y make
# Set the working directory inside the container
WORKDIR /app

# Copy the source code into the container
COPY . .
CMD ["make", "start"]
EXPOSE 3003