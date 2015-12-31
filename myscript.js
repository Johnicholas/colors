'use strict'

var renderer, stage, sprites;

function tick(ms) {
    Module.ccall('update', null, ['number'], [ms]);
    renderer.render(stage);
    requestAnimationFrame(tick);
}

function start() {
    renderer = new PIXI.WebGLRenderer(512, 512);
    stage = new PIXI.Container();
    sprites = [];
    renderer.view.onmousedown = function canvas_onmousedown(event) {
        var x = event.pageX - renderer.view.offsetLeft;
        var y = event.pageY - renderer.view.offsetTop;
        Module.ccall('mouse_down', null, ['number', 'number'], [x, y]);
    }
    renderer.view.onmouseup = function canvas_onmouseup(event) {
        var x = event.pageX - renderer.view.offsetLeft;
        var y = event.pageY - renderer.view.offsetTop;
        Module.ccall('mouse_up', null, ['number', 'number'], [x, y]);
    }
    renderer.view.onmousemove = function canvas_onmousemove(event) {
        var x = event.pageX - renderer.view.offsetLeft;
        var y = event.pageY - renderer.view.offsetTop;
        Module.ccall('mouse_move', null, ['number', 'number'], [x, y]);
    }
    renderer.view.oncontextmenu = function canvas_oncontextmenu(event) {
        return false;
    }
    Module.ccall('begin', null, [], []);
    document.body.appendChild(renderer.view);
    tick();
}

var Module = {
    preRun: [],
    postRun: [start],
    print: function (text) {
        console.log(text)
    },
    printErr: function (text) {
        console.error(text)
    },
    setStatus: function(text) {
        console.log('status: ' + text);
    },
    totalDependencies: 0,
    monitorRunDependencies: function(left) {
        console.log('monitorRunDependencies: ' + left);
    }
};
