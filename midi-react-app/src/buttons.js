import React from 'react';
import styled from 'styled-components';
import './buttons.css';
import Dropdown from "react-dropdown";
import Layout from './index.js';
import { toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';

const Button = styled.button`
    font-family: sans-serif;
    font-size: 1.3rem;
    border: 2px solid #3090c0;
    border-radius: 5px;
    padding: 7px 7px;
    display: block;
    margin: 0em;
    margin-left: 0.5em;
    margin-top: 1em;
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

export default class FileDialogue extends React.Component {
    state = {
        selectedInstrument: 'Remap instruments',
        selectedNote: 'Remap notes',
        selectedIndex: -1,
    }

    handleFileSelect = () => {
        const selectedFile = document.getElementById('file-upload').files[0];
        const reader = new FileReader();
        reader.readAsBinaryString(selectedFile);
        reader.onload = () => {
            console.log(reader.result);
        }
        // modify parseFile to parse a giant binary string
    }

    handleDirectorySelect = () => {
        const directory = document.getElementById('directory-upload').files;
        for (let i = 0; i < directory.length; i++) {
            const currFile = directory[i];
            const reader = new FileReader();
            reader.readAsBinaryString(currFile);
            reader.onload = () => {
                console.log(reader.result);
            }
            // send to parseFile (or make library really to build the directory)
        }
    }

    async setInstrument(instrument) {
        await this.setState({ selectedInstrument: instrument });
    }

    handleInstrumentChange = async (e) => {
        this.setInstrument(e.value);
        console.log(e.value);
        // insert api to access c data
    }

    handleNoteChange = () => {
        // insert api to access c data
    };

    updateSong = () => {
        // read all sliders, changed instruments, rewrite info to midi file
        toast.configure();
        toast('Song updated!', {position: toast.POSITION.TOP_RIGHT, autoClose: false})
    }

    async setIndex(index) {
        await this.setState({ selectedIndex: index });
    }

    removeSong = async () => {
        // this.setIndex(Layout.getSelectedIndex());
        // console.log(this.state.selectedIndex);
        // document.getElementById('list-item-0').remove();
        // change indices of other items in list

        const index = Layout.state.selectedIndex;
        this.setState({ selectedIndex: index });
        console.log(index);
    }

    render() {
        return (
            <div>
                <form className={'file-form'}>
                    <label htmlFor={'file-upload'} className={'custom-file-upload'}>Choose File</label>
                    <input id={'file-upload'} type={'file'} accept={'.mid'}
                           onChange={(e) => {
                               this.handleFileSelect()
                               e.target.value = null
                           }} multiple/>
                </form>
                <form className={'file-form'}>
                    <label htmlFor={'directory-uploads'} className={'custom-file-upload'}>Choose Directory</label>
                    <input directory={''} webkitdirectory={''} type={'file'} id={'directory-upload'}
                           onChange={(e) => {
                               this.handleDirectorySelect()
                               e.target.value = null
                           }} multiple/>
                </form>
                <script>
                </script>
                <Button onClick={this.updateSong}>Update Song</Button>
                <Button onClick={this.removeSong}>Remove Song</Button>
                <br/><br/><br/><br/><br/><br/><br/><br/><br/>
                <Dropdown options={instruments} onChange={this.handleInstrumentChange}
                          value={this.state.selectedInstrument} className={'dropdown'}/>
                <Dropdown options={notes} onChange={this.handleNoteChange}
                          value={this.state.selectedNote} className={'dropdown'}/>
            </div>
        );
    }
}
