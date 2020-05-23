import mido
from flask import Flask, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

path = "/Users/stephanie/midi-music-files/"


@app.route('/midi/<string:filename>/octaves/<int:num_octaves>', methods=['PUT'])
def change_octave(filename, num_octaves):
    mid = mido.MidiFile(path + filename, clip=True)
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                octave_increase = msg.note + (12 * num_octaves)
                if 0 <= octave_increase <= 127:
                    msg.note = octave_increase
    return 'success'


@app.route('/midi/<string:filename>/times/<float:multiplier>', methods=['PUT'])
def change_event_time(filename, multiplier):
    mid = mido.MidiFile(path + filename, clip=True)
    for track in mid.tracks:
        for msg in track:
            msg.time *= multiplier
    return 'success'


@app.route('/midi/<string:filename>/instruments/<int:instrument>', methods=['PUT'])
def change_event_instrument(filename, instrument):
    mid = mido.MidiFile(path + filename, clip=True)
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'program_change':
                msg.program = instrument
    return 'success'


@app.route('/midi/<string:filename>/notes/<int:note>', methods=['PUT'])
def change_event_note(filename, note):
    mid = mido.MidiFile(path + filename, clip=True)
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                msg.note = note
    return 'success'


@app.route('/midi/<string:filename>/song_info', methods=['GET'])
def range_of_song(filename):
    mid = mido.MidiFile(path + filename, clip=True)
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
