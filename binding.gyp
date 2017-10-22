#to compile with SDL2 instead of SDL1.2, use sdl2-config instead of sdl-config
{
    "targets": [
        {
            "target_name": "screenres",
            "sources": ["screenres.cc"],
            'cflags': ["-Wall", "-g", "-pedantic", "`sdl-config --cflags`", "-std=c++11"],
            "include_dirs": [ "<!(node -e \"require('nan')\")" ],
            'libraries': ["`sdl-config --libs`"], #NOTE: node-gyp ignores libs unless they are set here
        }
    ]
}
