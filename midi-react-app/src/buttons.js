import React from 'react';
import styled from 'styled-components';
import './buttons.css';
import Dropdown from "react-dropdown";
import { FilePicker } from 'react-file-picker';
import Layout from './index.js';

const Button = styled.button`
    font-family: sans-serif;
    font-size: 1.3rem;
    border: 2px solid #3090c0;
    border-radius: 5px;
    padding: 7px 7px;
    display: block;
    margin: 1em;
    text-align: center;
    width: 14rem;
    color: #3090c0;
    &:hover{
        transform: scale(1.1);
    }
`

const instruments = [
    {
        id: -1,
        value: 'None',
    },
    {
        id: 0,
        value: 'Acoustic Grand Piano',
    },
    {
        id: 4,
        value: 'Electric Piano',
    },
    {
        id: 6,
        value: 'Harpsichord',
    },
    {
        id: 9,
        value: 'Glockenspiel',
    },
    {
        id: 12,
        value: 'Marimba',
    },
    {
        id: 13,
        value: 'Xylophone',
    },
    {
        id: 20,
        value: 'Reed Organ',
    },
    {
        id: 21,
        value: 'Accordion',
    },
    {
        id: 24,
        value: 'Acoustic Guitar',
    },
    {
        id: 27,
        value: 'Electric Guitar',
    },
    {
        id: 31,
        value: 'Guitar harmonics',
    },
    {
        id: 32,
        value: 'Acoustic Bass',
    },
    {
        id: 40,
        value: 'Violin',
    },
    {
        id: 41,
        value: 'Viola',
    },
    {
        id: 42,
        value: 'Cello',
    },
    {
        id: 43,
        value: 'Contrabass',
    },
    {
        id: 44,
        value: 'Tremolo Strings',
    },
    {
        id: 45,
        value: 'Pizzicato Strings',
    },
    {
        id: 46,
        value: 'Orchestral Harp',
    },
    {
        id: 47,
        value: 'Timpani',
    },
    {
        id: 48,
        value: 'String Ensemble',
    },
    {
        id: 52,
        value: 'Choir Aahs',
    },
    {
        id: 53,
        value: 'Voice Oohs',
    },
    {
        id: 56,
        value: 'Trumpet',
    },
    {
        id: 57,
        value: 'Trombone',
    },
    {
        id: 58,
        value: 'Tuba',
    },
    {
        id: 60,
        value: 'French Horn',
    },
    {
        id: 61,
        value: 'Brass Section',
    },
    {
        id: 65,
        value: 'Alto Sax',
    },
    {
        id: 68,
        value: 'Oboe',
    },
    {
        id: 69,
        value: 'English Horn',
    },
    {
        id: 70,
        value: 'Bassoon',
    },
    {
        id: 71,
        value: 'Clarinet',
    },
    {
        id: 72,
        value: 'Piccolo',
    },
    {
        id: 73,
        value: 'Flute',
    },
    {
        id: 76,
        value: 'Blown Bottle',
    },
    {
        id: 78,
        value: 'Whistle',
    },
    {
        id: 103,
        value: 'Sci-Fi',
    },
    {
        id: 104,
        value: 'Sitar',
    },
    {
        id: 105,
        value: 'Banjo',
    },
    {
        id: 109,
        value: 'Bagpipes',
    },
    {
        id: 114,
        value: 'Steel Drum',
    },
    {
        id: 122,
        value: 'Seashore',
    },
    {
        id: 123,
        value: 'Bird Tweet',
    },
    {
        id: 124,
        value: 'Telephone Ring',
    },
    {
        id: 125,
        value: 'Helicopter',
    },
    {
        id: 126,
        value: 'Applause',
    },
    {
        id: 127,
        value: 'Gunshot',
    },
];

const notes = [
    {
        id: -2,
        value: 'No change',
    },
    {
        id: -1,
        value: '-1',
    },
    {
        id: 0,
        value: '0',
    },
    {
        id: 1,
        value: '1',
    },
    {
        id: 2,
        value: '2',
    },
    {
        id: 3,
        value: '3',
    },
];

function buildFileSelector() {
    const fileSelector = document.createElement('input');
    fileSelector.setAttribute('type', 'file');
    fileSelector.setAttribute('multiple', 'multiple');
    return fileSelector;
}

function buildDirectorySelector() {
    const directorySelector = document.createElement('input');
    directorySelector.setAttribute('directory', '');
    directorySelector.setAttribute('webkit-directory', '');
    directorySelector.setAttribute('type', 'file');
    directorySelector.setAttribute('id', 'directory');
    return directorySelector;
}

function doNothing() {}

export default class FileDialogue extends React.Component {
    state = {
        selectedSong: '',
    }

    componentDidMount() {
        this.fileSelector = buildFileSelector();
        this.directorySelector = buildDirectorySelector();
    }

    // handleFileSelect = (e) => {
    //     e.preventDefault();
    //     let fileSelector = this.fileSelector.click();
    // }

    handleFileSelect() {
        console.log('file selected');
    }

    handleDirectorySelect = (e) => {
        e.preventDefault();
        this.directorySelector.click();
    }

    handleInstrumentChange = () => {
        // insert api to access c data
    };

    handleNoteChange = () => {
        // insert api to access c data
    };

    removeSong = () => {
        const songToRemove = Layout.getSelectedSong
        console.log(songToRemove)
        // search list for list item with correactrect list text
        // get selected list item from list
        // remove selected song from list
    }

    render() {
        return (
            <div>
                {/*<Button onClick={this.handleFileSelect}>Choose File</Button>*/}
                <FilePicker
                    extensions={'.mid'}
                    onChange={this.handleFileSelect}
                    onError={errMsg => console.log(errMsg)}
                >
                    <Button>Choose File</Button>
                </FilePicker>
                <form className={'directory-button'}>
                    <label htmlFor={'directory-uploads'}>Choose Directory</label>
                    <input directory={''} webkitdirectory={''} type={'file'} id={'directory'} multiple/>
                </form>
                <script>
                </script>
                <Button onClick={this.handleDirectorySelect}>Choose Directory</Button>
                <Button onClick={doNothing}>Update Song</Button>
                <Button onClick={this.removeSong}>Remove Song</Button>
                <br/><br/><br/><br/>
                <Dropdown options={instruments} onChange={this.handleInstrumentChange} value={null}
                          placeholder={'Change instrument'} className={'dropdown'}/>
                <Dropdown options={notes} onChange={this.handleNoteChange} value={null}
                          placeholder={'Change octave'} className={'dropdown'}/>
            </div>
        );
    }
}
