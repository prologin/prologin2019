// Says that the godot replay should be in "loop"/"spectate" mode
const replay_loop = true;
const speed_value = 128;

// Read by godot.
var dump_value = "", players_value = {};

// Observe this to know then the next dump can be loaded.
var values_read = false;

var godot_is_loaded = false;

const INIT = 1, GETTING_INTERESTING = 2, PLAYING = 3, DELAYING = 4;

async function maybe_init_godot() {
    // Only init godot on *first* load
    if (godot_is_loaded) return;
    godot_is_loaded = true;

    console.log("initializing godot (once)");

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

    const engine = new Engine;
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

    // PROLOGIN 2019 -- RESIZE CANVAS
    const content_el = document.getElementById("content");
    const canvas_el = document.getElementById("canvas");

    const actual_width = 1280;
    const actual_height = 992;

    window.setInterval(function() {
        var width = content_el.offsetWidth;
        var height = (width / actual_width) * actual_height;

        if (document.fullscreen) {
            width = document.body.clientWidth;
            height = document.body.clientHeight;
        }

        canvas_el.setAttribute("style", `max-width: ${width}px; max-height: ${height}px;`);

        canvas_el.setAttribute("width", width);
        canvas_el.setAttribute("height", height);
    }, 500);
}

async function load_next_interesting() {
    console.log("downloading next interesting match");
    const data = await (await window.fetch('/api/matches/interesting/')).json();

    players_value = {};
    data.matchplayers.forEach(p => {
        players_value["" + p.id] = p.champion.name;
    });
    dump_value = await (await window.fetch(data.dump_url)).text();
    console.log("done loading interesting match");
}

function godot_becomes_ready() {
    return new Promise(function (resolve, reject) {
        console.log("en attendant godot…");
        setInterval(function() {
            if (values_read) {
                console.log("godot becomes ready");
                values_read = false;
                resolve();
            }
        }, 250);
    });
}

$(function() {
    (async function () {
        while (true) {
            await load_next_interesting();
            maybe_init_godot();
            await godot_becomes_ready();
        }
    })();
});
