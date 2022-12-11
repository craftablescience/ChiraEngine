# CMDL Files {#cmdlspec}
`.cmdl` is the file extension of the Chira Model file. The following info is contained within these files:
- Mesh info
- Material info
- Mesh Loader

No mesh, material, or texture data is present within CMDL files. only the info on what file should be used to get that info.

> WARNING: This file format has not been fully developed and is still being added onto. Features may be added and removed as time goes on.

[TOC]

## Format
Just like most other ChiraEngine specific resource info, CMDL files use a JSON format to store their data. Right now they only contain some basic information but the format is currently being expanded to provide more info such as
- Animations
- Actions
- Multiple Textures
- Multiple Meshes

## Properties
The following properties are currently implemented into the CMDL format

### Material Type
`Default Value: None`

This property determines what material type Chira will use when rendering the material defined in the [material](### Material) property.

Specifications:

| Spec | Value |
|----------|------------|
| Value Type | Material Type (String) |
| Optional | False |

### Material
`Default Value: None`

This property determines what material Chira will use when rendering the mesh defined in the [model](### Model) property.

Specifications:

| Spec | Value |
|----------|------------|
| Value Type | Resource Path (String) |
| Optional | False |

### Model
`Default Value: None`

This property determines what mesh Chira will load for this specific model

> **NOTE: Make sure the value you set in the [loader](### Loader) property matches the fomat of this mesh!**

Specifications:

| Spec | Value |
|----------|------------|
| Value Type | Resource Path (String) |
| Optional | False |

### Loader
`Default Value: None`

This property determines what type of mesh Chira will attempt to load the mesh defined in [model](### Model) as.

Specifications:

| Spec | Value |
|----------|------------|
| Value Type | Model Type Extension (String) |
| Supported Values| obj<br>cmsh |
| Optional | False |
