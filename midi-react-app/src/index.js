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
import PauseIcon from '@material-ui/icons/Pause';
import IconButton from '@material-ui/core/IconButton';
import {toast} from 'react-toastify';
import Select from 'react-select';
import MidiPlayer from 'web-midi-player';
// import ScriptTag from 'react-script-tag';
// import App from './App';
// import * as serviceWorker from './serviceWorker';

const url = 'http://127.0.0.1:5000/';
const instruments = [
    { label: 'None', value: -1 },
    { label: 'Acoustic Grand Piano', value: 0 },
    { label: 'Electric Piano', value: 4 },
    { label: 'Harpsichord', value: 6 },
    { label: 'Glockenspiel', value: 9 },
    { label: 'Marimba', value: 12 },
    { label: 'Xylophone', value: 13 },
    { label: 'Reed Organ', value: 20 },
    { label: 'Accordion', value: 21 },
    { label: 'Acoustic Guitar', value: 24 },
    { label: 'Electric Guitar', value: 27 },
    { label: 'Guitar harmonics', value: 31 },
    { label: 'Acoustic Bass', value: 32 },
    { label: 'Violin', value: 40 },
    { label: 'Viola', value: 41 },
    { label: 'Cello', value: 42 },
    { label: 'Contrabass', value: 43 },
    { label: 'Tremolo Strings', value: 44 },
    { label: 'Pizzicato Strings', value: 45 },
    { label: 'Orchestral Harp', value: 46 },
    { label: 'Timpani', value: 47 },
    { label: 'String Ensemble', value: 48 },
    { label: 'Choir Aahs', value: 52 },
    { label: 'Voice Oohs', value: 53 },
    { label: 'Trumpet', value: 56 },
    { label: 'Trombone', value: 57 },
    { label: 'Tuba', value: 58 },
    { label: 'French Horn', value: 60 },
    { label: 'Brass Section', value: 61 },
    { label: 'Alto Sax', value: 65 },
    { label: 'Oboe', value: 68 },
    { label: 'English Horn', value: 69 },
    { label: 'Bassoon', value: 70 },
    { label: 'Clarinet', value: 71 },
    { label: 'Piccolo', value: 72 },
    { label: 'Flute', value: 73 },
    { label: 'Blown Bottle', value: 76 },
    { label: 'Whistle', value: 78 },
    { label: 'Sci-Fi', value: 103 },
    { label: 'Sitar', value: 104 },
    { label: 'Banjo', value: 105 },
    { label: 'Bagpipes', value: 109 },
    { label: 'Steel Drum', value: 114 },
    { label: 'Seashore', value: 122 },
    { label: 'Bird Tweet', value: 123 },
    { label: 'Telephone Ring', value: 124 },
    { label: 'Helicopter', value: 125 },
    { label: 'Applause', value: 126 },
    { label: 'Gunshot', value: 127 },
]
const notes = [
    { label: 'No change', value: -1 },
    { label: 'Lower', value: 1 },
];
const customStyle = {
    control: (base, state) => ({
        ...base,
        border: state.isFocused ? 0 : 0,
        boxShadow: state.isFocused ? 0 : 0,
        '&:hover': {
            border: state.isFocused ? 0 : 0
        }
    })
};

let reverseInstruments = new Map();
let reverseNotes = new Map();

let eventLogger, player;

class Layout extends React.Component {
    state = {
        width: 0,
        height: 0,
        nowPLaying: '',
        previousPress: '',
        search: '',
        selectedSong: '',
        disabled: true,
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

        eventLogger = (payload) => {
            if (payload.event === 'MIDI_END') {
                this.setState({nowPlaying: ''});
            }
        };
        player = new MidiPlayer({eventLogger});

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
        await this.setState({ songs: songs, visibleSongs: songs, selectedIndex: -1 });
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
            let filesURL;
            fetch(url + currentSong, {'method': 'GET'})
                .then(response => response.json())
                .then(response => {
                    // no actual possibility of this occurring because selected song is an uploaded, existing file
                    if (response.path === 'file dne') {
                        alert('File does not exist.');
                        return;
                    }

                    if (response.path.includes("music-copy")) {
                        filesURL = 'http://localhost:8080/midis/music-copy/' + currentSong;
                    } else {
                        filesURL = 'http://localhost:8080/midis/' + currentSong;
                    }
                })
                .then(() => {
                    player.play({url: filesURL});

                    this.setState({nowPlaying: 'Now playing: ' + currentSong, previousPress: 'play'});
                });
        } else {
            alert('Please select a song.');
        }
    }

    pauseSong = () => {
        if (this.state.selectedIndex < 0 || this.state.selectedSong === '') {
            alert('Please select a song.');
            return;
        }
        const currentSong = this.state.selectedSong;

        if (this.state.previousPress === 'pause') {
            player.resume();
            this.setState({nowPlaying: 'Now playing: ' + currentSong, previousPress: 'resume'});
        } else if (this.state.previousPress === 'resume' || this.state.previousPress === 'play') {
            player.pause();
            this.setState({nowPlaying: '', previousPress: 'pause'});
        }
    }

    stopSong = () => {
        player.stop();
        this.setState({nowPlaying: '', previousPress: 'stop'});
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
        await this.setState({ selectedSong: e.target.innerText, selectedIndex: index, disabled: false });

        fetch(url + 'midi/' + this.state.selectedSong + '/song_info', {'method': 'GET'})
            .then(response => response.json())
            .then(response => {
                const description = 'Song info:\n' +
                    'File name: ' + this.state.selectedSong + '\n' +
                    'Note range: [' + response.low + ', ' + response.high + ']\n' +
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
                            <br/><br/><br/><br/><br/>
                            <label style={{width: this.state.width / 4 * .8}} className={'now-playing'}>
                                {this.state.nowPlaying}
                            </label>
                            <div style={{width: this.state.width / 4 * .8, display: 'inline',
                                marginLeft: this.state.width / 4 * .03}}>
                                <IconButton onClick={this.playSong} style={{
                                    color: 'white', margin: '1em', height: this.state.width / 4 * .18,
                                    marginTop: '0em', marginBottom: '0em', border: '2px solid #186090',
                                    backgroundColor: '#184878'
                                }}>
                                    <PlayArrowIcon style={{
                                        width: this.state.width / 4 * .09, height: this.state.width / 4 * .09
                                    }}/>
                                </IconButton>
                                <IconButton onClick={this.pauseSong} style={{
                                    color: 'white', margin: '1em', height: this.state.width / 4 * .18,
                                    marginTop: '0em', marginBottom: '0em', border: '2px solid #186090',
                                    backgroundColor: '#184878'
                                }}>
                                    <PauseIcon style={{
                                        width: this.state.width / 4 * .09, height: this.state.width / 4 * .09
                                    }}/>
                                </IconButton>
                                <IconButton onClick={this.stopSong} style={{
                                     color: 'white', margin: '1em', height: this.state.width / 4 * .18,
                                     marginTop: '0em', marginBottom: '0em', border: '2px solid #186090',
                                    backgroundColor: '#184878'
                                 }}>
                                     <StopIcon style={{
                                         width: this.state.width / 4 * .09, height: this.state.width / 4 * .09
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
                                   className={'range-slider'}
                                   onChange={(e) => {
                                        this.setState({octave: e.target.value});
                                   }}/>
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
                        <Select
                            value={null} placeholder={this.state.selectedInstrument} isDisabled={this.state.disabled}
                            onChange={(e) => {
                                this.setState({ selectedInstrument: e.label });
                            }}
                            options={instruments} className={'dropdown'} styles={customStyle} isSearchable={false}
                        />
                        <br/>
                        <label style={{width: this.state.width / 4 * .8, textAlign: 'left'}}>Remap notes:</label>
                        <Select
                            value={null} placeholder={this.state.selectedNote} isSearchable={false}
                            onChange={(e) => {
                                this.setState({ selectedNote: e.label });
                            }}
                            options={notes} className={'dropdown'} styles={customStyle} isDisabled={this.state.disabled}
                        />
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
