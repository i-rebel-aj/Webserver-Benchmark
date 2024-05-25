#!/bin/bash

# Temporary file to store success count
temp_file=$(mktemp)

# Initialize the temp file with 0
echo 0 > "$temp_file"

# Function to send HTTP request and check status
send_request() {
    local url=$1
    local response=$(curl -s -o /dev/null -w "%{http_code}" "$url")
    if [[ $response -eq 200 ]]; then
        # Increment success count atomically
        (
            flock -x 200
            count=$(<"$temp_file")
            echo $((count + 1)) > "$temp_file"
        ) 200>"$temp_file.lock"
    else
        echo "not ok"
    fi
}

# Number of requests to send
n=100
PORT=3003
# HTTP route to test
route="http://localhost:$PORT"
echo "Route where request is sent is $route"

# Send n number of requests in parallel
for ((i=1; i<=$n; i++)); do
    send_request "$route" &
done

# Wait for all requests to finish
wait

# Read the success count from the temp file
success_count=$(<"$temp_file")

# Clean up the temp file and lock file
rm "$temp_file" "$temp_file.lock"

echo "Total Requests $n, Success $success_count"
