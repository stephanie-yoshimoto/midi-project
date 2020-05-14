import React from 'react';
import ReactDOM from 'react-dom';
import 'react-dropdown/style.css';
import './index.css';
import FileDialogue from './buttons';
import Sliders from './sliders';
import {List, ListItem, ListItemText} from '@material-ui/core/';
// import App from './App';
// import * as serviceWorker from './serviceWorker';

class Layout extends React.Component {
    state = {
        search: '',
        selectedSong: '',
        selectedIndex: -1,
    }

    getSelectedSong() {
        return this.state.selectedSong
    }

    updateSearch(e) {
        this.setState({search: e.target.value});
    }

    updateSelectedSong = (e, index) => {
        this.setState({selectedIndex: index});
        document.getElementById('list-item-' + index).style.backgroundColor = '#78a8c0';
        for (let i = 0; i < index && document.getElementById('list-item-' + i) !== null; i++) {
            document.getElementById('list-item-' + i).style.backgroundColor = 'transparent';
        }
        for (let i = index + 1; document.getElementById('list-item-' + i) !== null; i++) {
            document.getElementById('list-item-' + i).style.backgroundColor = 'transparent';
        }
        // update selected song by using index
    }

    render() {
        return (
            <div>
                <h1 style={{textAlign: `center`}}>MIDI Library</h1>
                <div className={'top-block'}>
                    <div className={'buttons-dropdown'}>
                        <FileDialogue/>
                    </div>
                    <div className={'list'}>
                        <input type={'text'}
                               value={this.state.search}
                               placeholder={'Search Songs'}
                               onChange={this.updateSearch.bind(this)}/>
                        <List components={'nav'} aria-label={'song-list'}>
                            <ListItem button component={'a'} onClick={(e) => this.updateSelectedSong(e, 0)}
                                      id={'list-item-0'} className={'list-item'}>
                                <ListItemText primary={'prelude.mid'}/>
                            </ListItem>
                            <ListItem button component={'a'} onClick={(e) => this.updateSelectedSong(e, 1)}
                                      id={'list-item-1'} className={'list-item'}>
                                <ListItemText primary={'bohemianrhapsody.mid'}/>
                            </ListItem>
                        </List>
                    </div>
                </div>
                <div className={'sliders-div'}>
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

export default Layout;
