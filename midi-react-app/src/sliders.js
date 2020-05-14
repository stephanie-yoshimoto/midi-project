import React from 'react';

export default class Sliders extends React.Component {
    state = {
        timeScale: 10,
        warpTime: 1.0,
        octave: 0,
    }

    handleTimeScale(e) {
        this.setState({ timeScale: e.target.value} )
    }

    handleWarpTime(e) {
        this.setState({ warpTime: e.target.value} )
    }

    handleOctaveChange(e) {
        this.setState( {octave: e.target.value} )
    }

    render() {
        return (
            <div>
                <form className={'slider'}>
                    <label>Time Scale</label>
                    <input type={'range'} min={0} max={100} value={this.state.timeScale} step={10}
                           className={'range-slider'} onChange={this.handleTimeScale}/>
                </form>
                <form className={'slider'}>
                    <label>Warp Time</label>
                    <input type={'range'} min={0} max={100} value={this.state.warpTime} step={10}
                           className={'range-slider'} onChange={this.handleWarpTime}/>
                </form>
                <form className={'slider'}>
                    <label>Change Octave</label>
                    <input type={'range'} min={0} max={100} value={this.state.octave} step={10}
                           className={'range-slider'} onChange={this.handleOctaveChange}/>
                </form>
            </div>
        )
    }
}
