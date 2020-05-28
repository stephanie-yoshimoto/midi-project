import React from 'react';
import ReactDOM from 'react-dom';
import 'react-dropdown/style.css';
import './index.css';
import './buttons.css';
import './sliders.css';
import 'react-toastify/dist/ReactToastify.css';
import {List, ListItem} from '@material-ui/core/';
import StopIcon from '@material-ui/icons/Stop';
import PlayArrowIcon from '@material-ui/icons/PlayArrow';
import IconButton from '@material-ui/core/IconButton';
import Dropdown from 'react-dropdown';
import {toast} from "react-toastify";
// import App from './App';
// import * as serviceWorker from './serviceWorker';

const url = 'http://127.0.0.1:5000/';
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
        id: -1,
        value: 'No change',
    },
    {
        id: 1,
        value: 'Lower',
    },
];
let reverseInstruments = new Map();
let reverseNotes = new Map();

class Layout extends React.Component {
    state = {
        width: 0,
        height: 0,
        nowPlaying: '',
        search: '',
        selectedSong: '',
        selectedInstrument: 'Select instrument...',
        selectedNote: 'Select note change...',
        selectedIndex: -1,
        songs: [
            'bohemianrhapsody.mid',
            'pianoman.mid',
        ],
        visibleSongs: [
            'bohemianrhapsody.mid',
            'pianoman.mid',
        ],
        description: 'Song info:',
        warpTime: parseFloat('1').toFixed(1),
        octave: 0,
        lowestNote: 0,
        highestNote: 0,
        originalLength: 0,
    };

    updateDimensions = () => {
        this.setState({ width: window.innerWidth, height: window.innerHeight });
    }
    componentDidMount() {
        this.updateDimensions();
        window.addEventListener('resize', this.updateDimensions);

        reverseInstruments.set('Acoustic Grand Piano', 0);
        reverseInstruments.set('Electric Piano', 4);
        reverseInstruments.set('Harpsichord', 6);
        reverseInstruments.set('Glockenspiel', 9);
        reverseInstruments.set('Marimba', 12);
        reverseInstruments.set('Xylophone', 13);
        reverseInstruments.set('Reed Organ', 20);
        reverseInstruments.set('Accordion', 21);
        reverseInstruments.set('Acoustic Guitar', 24);
        reverseInstruments.set('Electric Guitar', 27);
        reverseInstruments.set('Guitar harmonics', 31);
        reverseInstruments.set('Acoustic Bass', 32);
        reverseInstruments.set('Violin', 40);
        reverseInstruments.set('Viola', 41);
        reverseInstruments.set('Cello', 42);
        reverseInstruments.set('Contrabass', 43);
        reverseInstruments.set('Tremolo Strings', 44);
        reverseInstruments.set('Pizzicato Strings', 45);
        reverseInstruments.set('Orchestral Harp', 46);
        reverseInstruments.set('Timpani', 47);
        reverseInstruments.set('String Ensemble', 48);
        reverseInstruments.set('Choir Aahs', 52);
        reverseInstruments.set('Voice Oohs', 53);
        reverseInstruments.set('Trumpet', 56);
        reverseInstruments.set('Trombone', 57);
        reverseInstruments.set('Tuba', 58);
        reverseInstruments.set('French Horn', 60);
        reverseInstruments.set('Brass Section', 61);
        reverseInstruments.set('Alto Sax', 65);
        reverseInstruments.set('Oboe', 68);
        reverseInstruments.set('English Horn', 69);
        reverseInstruments.set('Bassoon', 70);
        reverseInstruments.set('Clarinet', 71);
        reverseInstruments.set('Piccolo', 72);
        reverseInstruments.set('Flute', 73);
        reverseInstruments.set('Blown Bottle', 76);
        reverseInstruments.set('Whistle', 78);
        reverseInstruments.set('Sci-Fi', 103);
        reverseInstruments.set('Sitar', 104);
        reverseInstruments.set('Banjo', 105);
        reverseInstruments.set('Bagpipes', 109);
        reverseInstruments.set('Steel Drum', 114);
        reverseInstruments.set('Seashore', 122);
        reverseInstruments.set('Bird Tweet', 123);
        reverseInstruments.set('Telephone Ring', 124);
        reverseInstruments.set('Helicopter', 125);
        reverseInstruments.set('Applause', 126);
        reverseInstruments.set('Gunshot', 127);

        reverseNotes.set('Lower', 1);
    }
    componentWillUnmount() {
        window.removeEventListener('resize', this.updateDimensions);
    }

    handleFileSelect = () => {
        const files = document.getElementById('file-upload').files;
        for (let i = 0; i < files.length; i++) {
            const currFile = files[i];
            const reader = new FileReader();
            reader.readAsBinaryString(currFile);
            // reader.onload = () => {
            //     console.log(reader.result);
            // }
            let originalList = this.state.songs;
            if (originalList.indexOf(currFile.name) < 0) {
                originalList.push(currFile.name);
            } else {
                alert('ERROR! Song "' + currFile.name + '" already exists in list.');
            }
            this.setState({ songs: originalList, visibleSongs: originalList });
        }
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
            let originalList = this.state.songs;
            if (originalList.indexOf(currFile.name) < 0) {
                originalList.push(currFile.name);
            } else {
                alert('ERROR! Song "' + currFile.name + '" already exists in list.');
            }
            this.setState({ songs: originalList, visibleSongs: originalList });
        }
    }

    async updateList(songs) {
        await this.setState({ songs: songs, visibleSongs: songs });
    }

    removeSong = async () => {
        let index = this.state.selectedIndex;
        if (index >= 0) {
            let songsCopy = this.state.songs;
            songsCopy.splice(index, 1);
            this.updateList(songsCopy);
        } else {
            alert('Please select a song.');
        }
    }

    playSong = () => {
        const currentSong = this.state.selectedSong;
        if (currentSong !== '') {
            fetch(url +  'midi/' + currentSong + '/play', {'method': 'GET'})
                .then(response => response.json())
                .then(response => {
                    if (response.message === 'cannot play') {
                        alert('Cannot play song.');
                    } else {
                        this.setState({ nowPlaying:
                                'Now playing: ' + response.message
                        });
                    }
                });
        } else {
            alert('Please select a song.');
        }
    }

    stopSong = () => {
        fetch(url + 'midi/stop', {'method': 'GET'})
            .then(response => response.json())
            .then(response => {
                if (response.message === 'mixer not initialized') {
                    alert('No song started.');
                } else if (response.message === 'no music playing') {
                    alert('No music playing.');
                } else {
                    this.setState({ nowPlaying: '' });
                }
            });
    }

    updateSearch = (e) => {
        this.setState({ search: e.target.value });
        const searchKey = e.target.value.toLowerCase();
        let originalArray = [];

        // copy state array into local array
        for (let i = 0; i < this.state.songs.length; i++) {
            originalArray[i] = this.state.songs[i];
        }

        // change all elements of local array to lower case
        for (let i = 0; i < originalArray.length; i++) {
            originalArray[i] = originalArray[i].toLowerCase();
        }

        let newArray = [];
        for (let i = 0, j = 0; i < originalArray.length; i++) {
            if (originalArray[i].includes(searchKey)) {
                newArray[j] = i;
                j++;
            }
        }

        for (let i = 0; i < newArray.length; i++) {
            newArray[i] = this.state.songs[newArray[i]];
        }
        this.setState({ visibleSongs: newArray });
    }

    async changeState(e, index) {
        await this.setState({ selectedSong: e.target.innerText, selectedIndex: index });

        fetch(url + 'midi/' + this.state.selectedSong + '/song_info', {'method': 'GET'})
            .then(response => response.json())
            .then(response => {
                const description = 'Song info:\n' +
                    'File name: ' + this.state.selectedSong + '\n' +
                    'Note range: [' + response.low + ', ' + response.high+ ']\n' +
                    'Length: ' + response.length;
                this.setState({
                    description: description,
                    lowestNote: response.low,
                    highestNote: response.high,
                    length: response.length
                });
            })
            .catch(err => {
                console.log(err);
            });
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

    handleWarpTime = (e) => {
        const num = parseFloat(e.target.value);
        const cleanNum = num.toFixed(1);
        this.setState({ warpTime: cleanNum } );
    }

    handleOctaveChange = (e) => {
        this.setState( { octave: e.target.value } )
    }

    handleInstrumentChange = async (e) => {
        const instrument = e.value;
        this.setState({ selectedInstrument: instrument });
    }

    handleNoteChange = async (e) => {
        const note = e.value;
        this.setState({ selectedNote: note });
    };

    async update(warpTime, octave, negative, instrument, note) {
        if (warpTime < 1.0 || warpTime > 1.0) {
            await fetch(url + 'midi/' + this.state.selectedSong + '/times/' + warpTime, {'method': 'PUT'});
        }
        if (octave !== 0) {
            await fetch(url + 'midi/' + this.state.selectedSong + '/octaves/' + octave + '/' + negative,
                {'method': 'PUT'});
        }
        if (instrument != null) {
            await fetch(url + 'midi/' + this.state.selectedSong + '/instruments/' + instrument,
                {'method': 'PUT'});
        }
        if (note != null) {
            await fetch(url + 'midi/' + this.state.selectedSong + '/notes', {'method': 'PUT'});
        }
    }

    saveSong = async () => {
        let index = this.state.selectedIndex;
        if (index >= 0) {
            const warpTime = this.state.warpTime;

            let octave = this.state.octave;
            let negative = 'ignore';
            if (octave < 0) {
                octave *= -1;
                negative = 'negative';
            }

            const instrumentKey = this.state.selectedInstrument;
            const instrument = reverseInstruments.get(instrumentKey);

            const notesKey = this.state.selectedNote;
            const note = reverseNotes.get(notesKey);

            this.update(warpTime, octave, negative, instrument, note);
            toast.configure();
            toast.info('Song updated!', {position: 'top-right', autoClose: false});
        } else {
            alert('Please select a song.');
        }
    }

    render() {
        return (
            <div>
                <h1 style={{textAlign: `center`}}>MIDI Library</h1>
                <div className={'top-block'} style={{width: this.state.width, height: this.state.height}}>
                    <div className={'buttons-dropdown'}
                         style={{width: this.state.width / 4 * .95, height: this.state.height * .83}}
                    >
                        <div>
                            <form style={{width: this.state.width / 4 * .85}}>
                                <label htmlFor={'file-upload'} className={'custom-file-upload'}>Choose File</label>
                                <input id={'file-upload'} type={'file'} accept={'.mid'}
                                       onChange={(e) => {
                                           this.handleFileSelect()
                                           e.target.value = null
                                       }} multiple/>
                            </form>
                            <form style={{width: this.state.width / 4 * .85}}>
                                <label htmlFor={'directory-upload'} className={'custom-file-upload'}>
                                    Choose Directory
                                </label>
                                <input directory={''} webkitdirectory={''} type={'file'} id={'directory-upload'}
                                       onChange={(e) => {
                                           this.handleDirectorySelect()
                                           e.target.value = null
                                       }}/>
                            </form>
                            <button onClick={this.removeSong} style={{width: this.state.width / 4 * .85}}>
                                Remove Song
                            </button>
                            <br/><br/>
                            <label style={{width: this.state.width / 4 * .8}} className={'song-info'}>
                                {this.state.description}
                            </label>
                            <br/><br/><br/><br/>
                            <label style={{width: this.state.width / 4 * .8}} className={'now-playing'}>
                                {this.state.nowPlaying}
                            </label>
                            <div style={{width: this.state.width / 4 * .8, display: 'inline',
                                marginLeft: this.state.width / 4 * .1}}>
                                <IconButton onClick={this.playSong} style={{
                                    color: 'white', margin: '1em', height: this.state.width / 4 * .225,
                                    marginTop: '0em', border: '2px solid #186090', backgroundColor: '#184878'
                                }}>
                                    <PlayArrowIcon style={{
                                        width: this.state.width / 4 * .15, height: this.state.width / 4 * .15
                                    }}/>
                                </IconButton>
                                <IconButton onClick={this.stopSong} style={{
                                    color: 'white', margin: '1em', height: this.state.width / 4 * .225,
                                    marginTop: '0em', border: '2px solid #186090', backgroundColor: '#184878'
                                }}>
                                    <StopIcon style={{
                                        width: this.state.width / 4 * .15, height: this.state.width / 4 * .15
                                    }}/>
                                </IconButton>
                            </div>
                        </div>
                    </div>
                    <div className={'outer-list'} style={{width: this.state.width / 2 * .9,
                        height: this.state.height * .83}}>
                        <input type={'text'}
                               value={this.state.search}
                               placeholder={'Search Songs'}
                               onChange={this.updateSearch}
                               style={{width: (this.state.width / 2) * .87}}/>
                        <List className={'list'} style={{height: this.state.height * .75}}>
                            {this.state.visibleSongs.map(song => (
                                <ListItem button component={'a'} key={song}
                                          onClick={(e) => this.selectSong(e, this.state.visibleSongs.indexOf(song))}
                                          id={'list-item-' + this.state.visibleSongs.indexOf(song)}
                                          className={'list-item'} style={{width: this.state.width / 2 * .9}}>
                                    {song}
                                </ListItem>
                            ))}
                        </List>
                    </div>
                    <div className={'description'}
                         style={{width: this.state.width / 4 * .95, height: this.state.height * .83}}
                    >
                        <form className={'slider'} style={{width: this.state.width / 4 * .95}}>
                            <label style={{textAlign: `left`, width: this.state.width / 9 * .95}}>Warp Time</label>
                            <label style={
                                {width: this.state.width / 9, textAlign: `right`, color: `#78a8c0`, fontWeight: 900,
                                    fontSize: `2rem`}
                            }>
                                {this.state.warpTime}
                            </label>
                            <input type={'range'} min={0.1} max={10.0} value={this.state.warpTime} step={0.1}
                                   className={'range-slider'} onChange={this.handleWarpTime}/>
                            <br/>
                            <label className={'slider-range'}
                                   style={{textAlign: `left`, width: this.state.width / 9 * .95}}>
                                {0.1}
                            </label>
                            <label className={'slider-range'}
                                   style={{textAlign: `right`, width: this.state.width / 9 * .95}}>
                                {parseFloat('10').toFixed(1)}
                            </label>
                        </form>
                        <form className={'slider'} style={{width: this.state.width / 4 * .95}}>
                            <label style={{textAlign: `left`, width: this.state.width / 9 * .95}}>Change Octave</label>
                            <label style={
                                {width: this.state.width / 9, textAlign: `right`, color: `#78a8c0`, fontWeight: 900,
                                    fontSize: `2rem`}
                            }>
                                {this.state.octave}
                            </label>
                            <input type={'range'} min={-5} max={5} value={this.state.octave} step={1}
                                   className={'range-slider'} onChange={this.handleOctaveChange}/>
                            <br/>
                            <label className={'slider-range'}
                                   style={{textAlign: `left`, width: this.state.width / 9 * .95}}>
                                {-5}
                            </label>
                            <label className={'slider-range'}
                                   style={{textAlign: `right`, width: this.state.width / 9 * .95}}>
                                {5}
                            </label>
                        </form>
                        <label style={{width: this.state.width / 4 * .8, textAlign: 'left'}}>
                            Remap instruments:
                        </label>
                        <Dropdown options={instruments} onChange={this.handleInstrumentChange} value={null}
                                  placeholder={this.state.selectedInstrument} className={'dropdown'}
                                  style={{width: this.state.width / 4 * .85}}/>
                        <br/>
                        <label style={{width: this.state.width / 4 * .8, textAlign: 'left'}}>Remap notes:</label>
                        <Dropdown options={notes} onChange={this.handleNoteChange} value={null}
                                  placeholder={this.state.selectedNote} className={'dropdown'}
                                  style={{width: this.state.width / 4 * .85}}/>
                        {/*<select value={null} placeholder={this.state.selectedNote} className={'dropdown'}>*/}
                        {/*    <option value={-1}>No change</option>*/}
                        {/*    <option value={1}>Lower</option>*/}
                        {/*</select>*/}
                        <br/><br/><br/><br/>
                        <button onClick={this.saveSong} style={{width: this.state.width / 4 * .95}} className={'save'}>
                            Save Song
                        </button>
                    </div>
                </div>
            </div>
        );
    }
}

ReactDOM.render(
    <Layout/>,
    document.getElementById('root')
);

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
// serviceWorker.unregister();
