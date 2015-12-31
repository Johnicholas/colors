'use strict';

mergeInto(LibraryManager.library, {
    new_sprite: function(which, x, y, w, h, color) {
        var it = new PIXI.Graphics()
        it.beginFill(0xffffff);
        it.drawRect(x, y, w, h);
        it.endFill();
        it.tint = color;
        sprites[which] = it;
        stage.addChild(it);
    },
    set_sprite_position: function(which, x, y) {
        if (sprites[which]) {
            sprites[which].position.x = x;
            sprites[which].position.y = y;
        }
    },
    set_sprite_color: function(which, color) {
        if (sprites[which]) {
            sprites[which].tint = color;
        }
    },
    free_sprite: function(which) {
        if (sprites[which]) {
            stage.removeChild(sprites[which])
            delete sprites[which];
        }
    },
});
