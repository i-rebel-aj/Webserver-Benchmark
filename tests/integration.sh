#!/bin/bash

# Define the URL
URL="http://example.com"

# Define the routes to test
ROUTES=(
    "/route1"
    "/route2"
    "/route3"
)

# Define the expected status code
EXPECTED_STATUS_CODE=200

# Loop through the routes and make HTTP calls
for route in "${ROUTES[@]}"; do
    # Make the HTTP call and get the status code
    status_code=$(curl -s -o /dev/null -w "%{http_code}" "$URL$route")

    # Check if the status code matches the expected status code
    if [ "$status_code" -eq "$EXPECTED_STATUS_CODE" ]; then
        echo "Success: $route returned $status_code"
    else
        echo "Failure: $route returned $status_code"
    fi
done