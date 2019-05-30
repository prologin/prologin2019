// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2018 Sacha Delanoue
// Copyright 2019 Paul Guénézan
// Copyright 2014-2018 Godot Engine contributors

var dump_value = "the dump";

async function main() {
    $('#replay').html(`
        <div id="container">
            <canvas id="canvas" oncontextmenu="event.preventDefault();" width="640" height="480">
                HTML5 canvas appears to be unsupported in the current browser.<br />
                Please try updating or use a different browser.
            </canvas>
            <div id="status">
                <div id='status-progress' style='display: none;' oncontextmenu="event.preventDefault();"><div id ='status-progress-inner'></div></div>
                <div id='status-indeterminate' style='display: none;' oncontextmenu="event.preventDefault();">
                    <div></div>
                    <div></div>
                    <div></div>
                    <div></div>
                    <div></div>
                    <div></div>
                    <div></div>
                    <div></div>
                </div>
                <div id="status-notice" class="godot" style='display: none;'></div>
            </div>
        </div>`);

    $('body').append($('<script/>').attr('src', '/static/js/godot.js'));

    $.get("dump/")
        .then((game_data) => {
            dump_value = game_data;
            godot_init(new Engine);
        });
}

function godot_init(engine) {

    const BASENAME = '/static/godot/prologin2019';
    const INDETERMINATE_STATUS_STEP_MS = 100;

    var container = document.getElementById('container');
    var canvas = document.getElementById('canvas');
    var statusProgress = document.getElementById('status-progress');
    var statusProgressInner = document.getElementById('status-progress-inner');
    var statusIndeterminate = document.getElementById('status-indeterminate');
    var statusNotice = document.getElementById('status-notice');

    var initializing = true;
    var statusMode = 'hidden';
    var indeterminiateStatusAnimationId = 0;

    setStatusMode('indeterminate');
    engine.setCanvas(canvas);

    function setStatusMode(mode) {

        if (statusMode === mode || !initializing)
            return;
        [statusProgress, statusIndeterminate, statusNotice].forEach(elem => {
            elem.style.display = 'none';
        });
        if (indeterminiateStatusAnimationId !== 0) {
            cancelAnimationFrame(indeterminiateStatusAnimationId);
            indeterminiateStatusAnimationId = 0;
        }
        switch (mode) {
            case 'progress':
                statusProgress.style.display = 'block';
                break;
            case 'indeterminate':
                statusIndeterminate.style.display = 'block';
                indeterminiateStatusAnimationId = requestAnimationFrame(animateStatusIndeterminate);
                break;
            case 'notice':
                statusNotice.style.display = 'block';
                break;
            case 'hidden':
                break;
            default:
                throw new Error("Invalid status mode");
        }
        statusMode = mode;
    }

    function animateStatusIndeterminate(ms) {
        var i = Math.floor(ms / INDETERMINATE_STATUS_STEP_MS % 8);
        if (statusIndeterminate.children[i].style.borderTopColor == '') {
            Array.prototype.slice.call(statusIndeterminate.children).forEach(child => {
                child.style.borderTopColor = '';
            });
            statusIndeterminate.children[i].style.borderTopColor = '#dfdfdf';
        }
        requestAnimationFrame(animateStatusIndeterminate);
    }

    function setStatusNotice(text) {

        while (statusNotice.lastChild) {
            statusNotice.removeChild(statusNotice.lastChild);
        }
        var lines = text.split('\n');
        lines.forEach((line, index) => {
            statusNotice.appendChild(document.createTextNode(line));
            statusNotice.appendChild(document.createElement('br'));
        });
    };

    engine.setProgressFunc((current, total) => {

        if (total > 0) {
            statusProgressInner.style.width = current/total * 100 + '%';
            setStatusMode('progress');
            if (current === total) {
                // wait for progress bar animation
                setTimeout(() => {
                    setStatusMode('indeterminate');
                }, 500);
            }
        } else {
            setStatusMode('indeterminate');
        }
    });

    engine.startGame(BASENAME + '.pck').then(() => {
        setStatusMode('hidden');
        initializing = false;
    }, err => {
        setStatusNotice(err.message);
        setStatusMode('notice');
        initializing = false;
    });
}

$(function () {
    (async function () {
        main();
    })();
});
