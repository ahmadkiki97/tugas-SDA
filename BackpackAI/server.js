const express = require('express');
const path = require('path');

const app = express();
const port = process.env.PORT || 3000;
const webRoot = path.join(__dirname, 'web_frontend');

app.use(express.static(webRoot));

app.get('/', (req, res) => {
  res.sendFile(path.join(webRoot, 'index.html'));
});

app.listen(port, () => {
  console.log(`BackpackAI running at http://127.0.0.1:${port}`);
});
