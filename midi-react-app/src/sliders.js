import React from 'react';
import './sliders.css'

export default class Sliders extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            timeScale: 1,
            warpTime: parseFloat('1').toFixed(1),
            octave: 0,
        }
    }

    handleTimeScale = (e) => {
        this.setState({ timeScale: e.target.value })
    }

    handleWarpTime = (e) => {
        const num = parseFloat(e.target.value);
        const cleanNum = num.toFixed(1);
        this.setState({ warpTime: cleanNum } );
    }

    handleOctaveChange = (e) => {
        this.setState( { octave: e.target.value } )
    }

    render() {
        return (
            <div>
                <form className={'slider'}>
                    <label>Time Scale</label>
                    <label style={
                        {width: `9rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}
                    }>
                        {this.state.timeScale}
                    </label>
                    <input type={'range'} min={1} max={10000} value={this.state.timeScale} step={1}
                           className={'range-slider'} onChange={this.handleTimeScale}/>
                    <br/>
                    <label className={'slider-range'} style={{textAlign: `left`}}>{1}</label>
                    <label className={'slider-range'} style={{textAlign: `right`}}>{10000}</label>
                </form>
                <form className={'slider'}>
                    <label>Warp Time</label>
                    <label style={
                        {width: `9rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}
                    }>
                        {this.state.warpTime}
                    </label>
                    <input type={'range'} min={0.1} max={10.0} value={this.state.warpTime} step={0.1}
                           className={'range-slider'} onChange={this.handleWarpTime}/>
                    <br/>
                    <label className={'slider-range'} style={{textAlign: `left`}}>{0.1}</label>
                    <label className={'slider-range'}
                           style={{textAlign: `right`}}>{parseFloat('10').toFixed(1)}</label>
                </form>
                <form className={'slider'}>
                    <label>Change Octave</label>
                    <label style={
                        {width: `7rem`, textAlign: `right`, color: `#78a8c0`, fontWeight: 900, fontSize: `2rem`}
                    }>
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
