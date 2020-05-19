import React from 'react';
import ReactDOM from 'react-dom';
import 'react-dropdown/style.css';
import './index.css';
import './buttons.css';
import 'react-toastify/dist/ReactToastify.css';
import Sliders from './sliders';
import {List, ListItem} from '@material-ui/core/';
import DrawingArea from "./DrawingArea";
import Dropdown from "react-dropdown";
import {toast} from "react-toastify";
import styled from "styled-components";
// import App from './App';
// import * as serviceWorker from './serviceWorker';

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

class Layout extends React.Component {
    state = {
        search: '',
        selectedSong: '',
        selectedInstrument: 'Select instrument...',
        selectedNote: 'Select note change...',
        selectedIndex: -1,
        songs: [
            'billiejean.mid',
            'prelude2.mid',
        ],
        visibleSongs: [
            'billiejean.mid',
            'prelude2.mid',
        ]
    };

    handleFileSelect = () => {
        const selectedFile = document.getElementById('file-upload').files[0];
        const reader = new FileReader();
        reader.readAsBinaryString(selectedFile);
        // reader.onload = () => {
        // console.log(reader.result);
        // }
        // modify parseFile to parse a giant binary string
        let originalList = this.state.songs;
        if (originalList.indexOf(selectedFile.name) < 0) {
            originalList.push(selectedFile.name);
        } else {
            alert('ERROR! Song "' + selectedFile.name + '" already exists in list.');
        }
        this.setState({ songs: originalList, visibleSongs: originalList });
    }

    handleDirectorySelect = () => {
        const directory = document.getElementById('directory-upload').files;
        for (let i = 0; i < directory.length; i++) {
            const currFile = directory[i];
            if (currFile.name[0] === '.') {
                // skips hidden files (files with filename starting with .)
                continue;
            }
            const reader = new FileReader();
            reader.readAsBinaryString(currFile);
            // reader.onload = () => {
            //     console.log(reader.result);
            // }
            // send to parseFile (or make library really to build the directory)
            let originalList = this.state.songs;
            if (originalList.indexOf(currFile.name) < 0) {
                originalList.push(currFile.name);
            } else {
                alert('ERROR! Song "' + currFile.name + '" already exists in list.');
            }
            this.setState({ songs: originalList, visibleSongs: originalList });
        }
    }

    handleInstrumentChange = async (e) => {
        const instrument = e.value;
        this.setState({ selectedInstrument: instrument });
        for (let i = 0; i < instruments.length; i++) {

        }
        // insert api to access c data
    }

    handleNoteChange = async (e) => {
        const note = e.value;
        this.setState({ selectedNote: note });
        // insert api to access c data
    };

    updateSong = () => {
        // read all sliders, changed instruments, rewrite info to midi file
        toast.configure();
        toast('Song updated!', {position: toast.POSITION.TOP_RIGHT, autoClose: false})
    }

    async updateList(songs) {
        await this.setState({ songs: songs, visibleSongs: songs });
    }

    removeSong = async () => {
        let index = this.state.selectedIndex;
        let songsCopy = this.state.songs;
        songsCopy.splice(index, 1);
        this.updateList(songsCopy);
    }

    updateSearch = (e) => {
        this.setState({ search: e.target.value });
        let newArray = this.state.songs.filter((d)=>{
            return d.indexOf(e.target.value) !== -1
        });
        this.setState({ visibleSongs: newArray });
    }

    async changeState(e, index) {
        await this.setState({ selectedSong: e.target.innerText, selectedIndex: index });
    }

    selectSong = async (e, index) => {
        this.changeState(e, index);
        for (let i = 0; i < index && document.getElementById('list-item-' + i) !== null; i++) {
            document.getElementById('list-item-' + i).style.backgroundColor = 'transparent';
        }
        let temp = index++;
        index--;
        for (let i = temp; document.getElementById('list-item-' + i) !== null; i++) {
            document.getElementById('list-item-' + i).style.backgroundColor = 'transparent';
        }
        document.getElementById('list-item-' + index).style.backgroundColor = '#78a8c0';
    }

    render() {
        return (
            <div>
                <h1 style={{textAlign: `center`}}>MIDI Library</h1>
                <div className={'top-block'}>
                    <div className={'buttons-dropdown'}>
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
                                <label htmlFor={'directory-upload'} className={'custom-file-upload'}>
                                    Choose Directory
                                </label>
                                <input directory={''} webkitdirectory={''} type={'file'} id={'directory-upload'}
                                       onChange={(e) => {
                                           this.handleDirectorySelect()
                                           e.target.value = null
                                       }} multiple/>
                            </form>
                            <Button onClick={this.updateSong}>Update Song</Button>
                            <Button onClick={this.removeSong}>Remove Song</Button>
                            <br/><br/><br/><br/><br/>
                            <label>Remap instruments:</label>
                            <Dropdown options={instruments} onChange={this.handleInstrumentChange} value={null}
                                      placeholder={this.state.selectedInstrument} className={'dropdown'}/>
                            <br/><br/>
                            <label>Remap notes:</label>
                            <Dropdown options={notes} onChange={this.handleNoteChange} value={null}
                                      placeholder={this.state.selectedNote} className={'dropdown'} selection/>
                        </div>
                    </div>
                    <div className={'list'}>
                        <input type={'text'}
                               value={this.state.search}
                               placeholder={'Search Songs'}
                               onChange={this.updateSearch}/>
                        <List>
                            {this.state.visibleSongs.map(song => (
                                <ListItem button component={'a'} key={song}
                                          onClick={(e) => this.selectSong(e, this.state.visibleSongs.indexOf(song))}
                                          id={'list-item-' + this.state.visibleSongs.indexOf(song)}
                                          className={'list-item'}>
                                    {song}
                                </ListItem>
                            ))}
                        </List>
                    </div>
                    <div className={'drawing-div'}>
                        <br/><br/><br/><br/>
                        <label>Drawing Area:</label>
                        <DrawingArea />
                    </div>
                </div>
                <div>
                    <Sliders />
                </div>
            </div>
        );
    }
}

ReactDOM.render(
    // <React.StrictMode>
    //   <App />
    // </React.StrictMode>,
    <Layout/>,
    document.getElementById('root')
);

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
// serviceWorker.unregister();
