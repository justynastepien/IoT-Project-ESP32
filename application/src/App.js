import './App.css';
import React, { useEffect, useState } from 'react';
import Graph from './components/graph.tsx';
import CustomAlert from './components/alert.tsx';
import CustomAlertPres from './components/alert_pres.tsx';
import GraphPres from './components/graph_pres.tsx';

function App() {
  const [data, setData] = useState([]);

  useEffect(() => {
    const interval = setInterval(() => {
      console.log('Getting data from api');

      fetch('http://localhost:8080/data')
        .then((response) => response.json())
          .then((new_data) => {
            console.log("got data:", new_data,  data);
            setData(data.concat(new_data)); 
          })
    }, 3000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="App">
      {data.length != 0 ? (
      <div class="contrainer">
      <h1>Pomiary czujnika temperatury i ciśnienia</h1>
        <div class="row justify-content-center">
          <div class="col-4">
            <CustomAlert data={{tytul: "Temperatura", opis: "Optymalne wartości są między 15 a 25 st C.", aktualne: data.at(-1).temperature + " \u00b0 C"}}/>
          </div>
          <div class="col-4">
            <CustomAlertPres data={{tytul: "Ciśnienie", opis: "Optymalne wartości są między 999 a 1020 hPa.", aktualne: data.at(-1).pressure + " hPa"}}/>
          </div>
        </div>
        <div class="row justify-content-center">
          <div class="col-4">
            <Graph data={{dane: data, key: "temperature"}}/>
          </div>
          <div class="col-4">
            <GraphPres data={{dane: data, key: "pressure"}}/>
          </div>
        </div>
      </div>
      ) : (<></>)}
    </div>
  );
}

export default App;
