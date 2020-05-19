import React from 'react';
import { Stage, Layer, Line } from 'react-konva';

const style = {
    border: '2px solid black',
    borderRadius: '5px',
    margin: '2em',
    marginTop: '.75em',
};

const constants = {
    WIDTH: 600,
    HEIGHT: 400,
};

export default class DrawingArea extends React.Component {
    render() {
        return (
            <Stage width={constants.WIDTH} height={constants.HEIGHT} style={style} className={'stage'}>
                <Layer>
                    <Line
                        points={[0, constants.HEIGHT / 2, constants.WIDTH, constants.HEIGHT / 2]}
                        stroke={'black'}/>
                </Layer>
            </Stage>
        );
    }
}

// const color_specs = {
//     piano: "#ff0000",
//     chromaticPercussion: "#8b4513",
//     organ: "#800080",
//     guitar: "#00ff00",
//     bass: "#0000ff",
//     strings: "#00ffff",
//     ensemble: "#008080",
//     brass: "#ffa500",
//     reed: "#ff00ff",
//     pipe: "#ffff00",
//     synthLead: "#4b0082",
//     synthPad: "#2f4f4f",
//     synthEffects: "#c0c0c0",
//     ethnic: "#808000",
//     percussive: "#c0c0c0",
//     soundEffects: "#808080",
// };