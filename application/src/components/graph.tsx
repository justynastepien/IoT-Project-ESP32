import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from 'recharts';

const Graph = ({data}: any) => {
  return (
      <LineChart
          width={500}
          height={300}
          data={data.dane}
          margin={{  
          top: 5,
          right: 20,
          left: 20,
          bottom: 5,
          }}>
          <CartesianGrid />
          <YAxis type="number" domain={[10, 28]}/>
          <Tooltip />
          <Legend />
          <Line type="monotone" dataKey={data.key} activeDot={{ r: 8 }} />
      </LineChart>
  )
}

export default Graph;