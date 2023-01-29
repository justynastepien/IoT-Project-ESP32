import React from 'react';

const CustomAlert = ({data}: any) => {
    let current = data.aktualne.substring(0, data.aktualne.indexOf(' '));
    let alertType2 = "alert alert-success"
    if(current < 15 || current > 25) {
        alertType2 = "alert alert-warning"
    } else {
        alertType2 = "alert alert-success"
    }
    // let alertType = current < 50 ? "alert alert-success" : "alert alert-warning"
    return (
        <div className={alertType2} style={{marginTop: 20}}>
            <h4 className="alert-heading">{data.tytul}</h4>
            <p>{data.opis}</p>
            <hr/>
            <p className="mb-0"><h5>{data.aktualne}</h5></p>
        </div>
    )
}

export default CustomAlert;