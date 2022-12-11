# CSCN Files {#cscnspec}
This file explains in detail exactly how the ChiraEngine Scene files (`.cscn`) work and what most if not all fields do and what their possible values (if applicable) are.

[TOC]

## Format
The Scene file itself is written in JSON meaning it's quite easy to parse. Allowing the creation of external editors for game specific tools (EX. No Return's Bismuth Editor) to be easier than most. only needing a JSON parser to actually read data out of the file. 

How this data is used is left entirely up to the creator of the tool or game as most values are stored in the most basic manner possible to allow for maximum amount of ease in creating other tools.

## Example scene file
Here is an example of a scene file that contains a simple blue sky environment and a single cube object on a static plane.<br>
Due to the simplicity of this example this doesn't show off a good chunk of the values that exist. This is only to be used as a basic example to visually see how these files are structured.

> NOTE: Actual `.cscn` files cannot host comments. They only exist here to provide pointers as to what is what at a glace.

```JSON
{
    "Entities": [
        {
            "id": "Example Entity",
            "type": "MeshEntity",
            "trsnf": {
                "pos": [0,0,0],
                "scale": [0,0,0],
                "rotate": [0,0,0],
            },
        },
        //...
    ],
    "LevelData": [
        {
            // TODO: Create a system for level data (Most likely something like the source 2 mesh system?)
        },
     ]
}
```

# SceneInfo
`TODO: Implement these values as support is added`

# Entities
The Entities property is used to store a hierarchical, order respecting list of entities inside of a scene. Each entity is placed into the list at the exact position that it's placed inside of the Editor's visual entities dock.

The list below details the structure that makes up a given entity.

|