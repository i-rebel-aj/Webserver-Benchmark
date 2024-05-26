const axios = require('axios');

// Function to send HTTP request and check status
async function sendRequest(url) {
    try {
        const response = await axios.get(url, { timeout: 10000 });
        return response.status === 200;
    } catch (error) {
	    console.log("error is", error.message)
        return false;
    }
}

// Number of requests to send
const n = 10;
const PORT = 3003;
// HTTP route to test
const route = `http://localhost:${PORT}`;
console.log(`Route where request is sent is ${route}`);

// Function to start sending requests
async function startLoadTest() {
    const promises = [];
    for (let i = 0; i < n; i++) {
        	console.log(`Request ${i} sent at ${Date.now()}`)
	    promises.push(sendRequest(route));
    }

    const results = await Promise.all(promises);
    const successCount = results.filter(result => result).length;
    console.log(`Total Requests ${n}, Success ${successCount}`);
}

// Start the load test
startLoadTest();
