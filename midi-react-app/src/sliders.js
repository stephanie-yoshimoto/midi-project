import React from 'react';
import './sliders.css'

export default class Sliders extends React.Component {
    state = {
        timeScale: 0,
        warpTime: 0.0,
        octave: 0,
    }

    handleTimeScale = (e) => {
        this.setState({ timeScale: e.target.value })
    }

    handleWarpTime = (e) => {
        this.setState({ warpTime: e.target.value} )
    }

    handleOctaveChange = (e) => {
        this.setState( { octave: e.target.value } )
    }

    render() {
        return (
            <div>
                <form className={'slider'}>
                    <label>Time Scale</label>
                    <label
                        style={{width: `9rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}}
                    >
                        {this.state.timeScale}
                    </label>
                    <input type={'range'} min={0} max={10000} value={this.state.timeScale} step={1000}
                           className={'range-slider'} onChange={this.handleTimeScale}/>
                    <br/>
                    <label className={'slider-range'} style={{textAlign: `left`}}>{0}</label>
                    <label className={'slider-range'} style={{textAlign: `right`}}>{10000}</label>
                </form>
                <form className={'slider'}>
                    <label>Warp Time</label>
                    <label
                        style={{width: `9rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}}
                    >
                        {this.state.warpTime}
                    </label>
                    <input type={'range'} min={0.0} max={10.0} value={this.state.warpTime} step={1.0}
                           className={'range-slider'} onChange={this.handleWarpTime}/>
                    <br/>
                    <label className={'slider-range'} style={{textAlign: `left`}}>{0.0}</label>
                    <label className={'slider-range'} style={{textAlign: `right`}}>{10.0}</label>
                </form>
                <form className={'slider'}>
                    <label>Change Octave</label>
                    <label
                        style={{width: `7rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}}
                    >
                        {this.state.octave}
                    </label>
                    <input type={'range'} min={-5} max={5} value={this.state.octave} step={1}
                           className={'range-slider'} onChange={this.handleOctaveChange}/>
                    <br/>
                    <label className={'slider-range'} style={{textAlign: `left`}}>{-5}</label>
                    <label className={'slider-range'} style={{textAlign: `right`}}>{5}</label>
                </form>
            </div>
        )
    }
}
