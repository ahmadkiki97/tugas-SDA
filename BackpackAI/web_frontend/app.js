const API_BASE = "http://127.0.0.1:8000";

// DOM Elements
const searchBtn = document.getElementById('searchBtn');
const btnText = document.getElementById('btnText');
const btnLoading = document.getElementById('btnLoading');
const resultDiv = document.getElementById('result');
const errorDiv = document.getElementById('error');
const routeResult = document.getElementById('routeResult');
const algoInfo = document.getElementById('algoInfo');
const cityStatus = document.getElementById('cityStatus');

// Modals
const citiesModal = document.getElementById('citiesModal');
const searchCityModal = document.getElementById('searchCityModal');
const bigOModal = document.getElementById('bigOModal');

// Search Route
searchBtn.addEventListener('click', async () => {
    const asal = document.getElementById('asal').value;
    const tujuan = document.getElementById('tujuan').value;
    const algo = document.getElementById('algo').value;

    btnText.style.display = 'none';
    btnLoading.style.display = 'inline';
    resultDiv.style.display = 'none';
    errorDiv.style.display = 'none';

    try {
        const response = await fetch(`${API_BASE}/cari_rute?asal=${asal}&tujuan=${tujuan}&algo=${algo}`);
        const data = await response.json();

        if (data.status === 'success') {
            routeResult.innerText = data.hasil_pencarian;
            algoInfo.innerText = algo;
            cityStatus.innerText = "Connected";
            resultDiv.style.display = 'block';
        } else {
            throw new Error(data.detail || "Failed");
        }
    } catch (err) {
        errorDiv.style.display = 'block';
        document.getElementById('errorText').innerText = err.message;
    } finally {
        btnText.style.display = 'inline';
        btnLoading.style.display = 'none';
    }
});

// View Cities
document.getElementById('citiesBtn').addEventListener('click', async () => {
    citiesModal.style.display = 'block';
    const list = document.getElementById('citiesList');
    try {
        const response = await fetch(`${API_BASE}/cities`);
        const data = await response.json();
        list.innerHTML = data.cities.map(c => `<span class="city-badge">${c}</span>`).join('');
    } catch (e) {
        list.innerText = "Failed to load cities.";
    }
});

// Big O
document.getElementById('bigOBtn').addEventListener('click', async () => {
    bigOModal.style.display = 'block';
    const res = document.getElementById('bigOResult');
    try {
        const response = await fetch(`${API_BASE}/bigo`);
        const data = await response.json();
        res.innerText = data.big_o_notation;
    } catch (e) {
        res.innerText = "Failed to load analysis.";
    }
});

// Search City
document.getElementById('searchCityBtn').addEventListener('click', () => {
    searchCityModal.style.display = 'block';
});

document.getElementById('citySearchBtn').addEventListener('click', async () => {
    const keyword = document.getElementById('citySearchInput').value;
    const res = document.getElementById('searchCityResult');
    try {
        const response = await fetch(`${API_BASE}/search?keyword=${keyword}`);
        const data = await response.json();
        res.innerText = data.hasil_pencarian;
    } catch (e) {
        res.innerText = "Error searching city.";
    }
});

// Modal Close
document.querySelectorAll('.close-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        citiesModal.style.display = 'none';
        searchCityModal.style.display = 'none';
        bigOModal.style.display = 'none';
    });
});
