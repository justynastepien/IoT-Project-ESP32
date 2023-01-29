import React from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from 'recharts';

const GraphPres = ({data}: any) => {
  return (
      <LineChart
          width={500}
          height={300}
          data={data.dane}
          margin={{
          top: 5,
          right: 20,
          left: 0,
          bottom: 5,
          }}>
          <CartesianGrid />
          <YAxis type="number" domain={[950, 1010]}/>
          <Tooltip />
          <Legend />
          <Line type="monotone" dataKey={data.key} activeDot={{ r: 8 }} />
      </LineChart>
  )
}

export default GraphPres;