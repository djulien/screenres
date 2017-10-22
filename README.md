
# screenres

Get and set screen resolutions

## Dependencies

Install SDL1.2 first.  For example, on Linux use the command:
```sh
sudo apt-get install libsdl1.2-dev
```

(Changing screen res doesn't work yet with SDL2 because it screenres would need to create a new window)

NOTE from https://www.raspberrypi.org/forums/viewtopic.php?t=13789 :
seg fault over SSH (framebuffer not accessible in SSH session).
It must run directly from console.

## Usage

### Programmatic

```sh
$ npm install screenres --save
```

#### get

```js
var sr = require('screenres');
console.log(sr.get()); // [1440, 900]
```

#### set

```js
var sr = require('screenres');
sr.set(800, 600));
```

### CLI

```sh
$ npm install -g screenres
```

#### help

```sh
$ screenres --help
```

#### get

```sh
$ screenres
1440x900
```


#### set

```sh
$ screenres 800 600
```

## Platforms

Any platform supported by SDL1.2.

## Contributing

PRs welcome.

## Kudos

Previous version was sponsored by [nearForm](http://nearform.com)




