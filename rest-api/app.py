import mido
import os
from flask import Flask, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

global path


@app.route('/<string:filename>', methods=['GET'])
def browser_request(filename):
    for dirpath, dirnames, filenames in os.walk('/Users/stephanie/midi-music-files'):
        if filename in filenames:
            global path
            path = dirpath + '/' + filename
            return jsonify({'path': path})
    return jsonify({'path': 'file dne'})


def get_file_path(filename):
    for dirpath, dirnames, filenames in os.walk('/Users/stephanie/midi-music-files'):
        if filename in filenames:
            global path
            path = dirpath + '/' + filename
            return path
    return 'file dne'


@app.route('/midi/<string:filename>/times/<float:multiplier>', methods=['PUT'])
def change_event_time(filename, multiplier):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    for track in mid.tracks:
        for msg in track:
            msg.time *= multiplier
            msg.time = int(msg.time)
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/octaves/<int:num_octaves>/<string:negative>', methods=['PUT'])
def change_octave(filename, num_octaves, negative):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    if negative == 'negative':
        num_octaves *= -1

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                octave_increase = msg.note + (12 * num_octaves)
                if 0 <= octave_increase <= 127:
                    msg.note = octave_increase
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/instruments/<int:instrument>', methods=['PUT'])
def change_event_instrument(filename, instrument):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'program_change':
                msg.program = instrument
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/notes', methods=['PUT'])
def change_event_note(filename):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    # build remapping
    lower = [0] * 0x100
    for i in range(1, 0x100, 12):
        lower[i] = i - 1
    for i in range(6, 0x100, 12):
        lower[i] = i + 1

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                msg.note = lower[msg.note]

    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/song_info', methods=['GET'])
def range_of_song(filename):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return jsonify({'low': 0, 'high': 0, 'length': 0})
    except IndexError:
        # midi file incorrectly formatted
        return jsonify({'low': 0, 'high': 0, 'length': 0})
    except IOError:
        # file does not exist
        return jsonify({'low': 0, 'high': 0, 'length': 0})

    lowest_note = 127
    highest_note = 0
    events_length = 0
    original_length = 0
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                if msg.note < lowest_note:
                    lowest_note = msg.note
                if msg.note > highest_note:
                    highest_note = msg.note

            # regardless of type of event, include the delta time in events_length
            events_length += msg.time

        # for each track, check if total delta times within track are greater than a previous track's delta time
        if events_length > original_length:
            original_length = events_length

        # reset events_length to count for next track
        events_length = 0

    return jsonify({'low': lowest_note, 'high': highest_note, 'length': original_length})


if __name__ == '__main__':
    app.run()
