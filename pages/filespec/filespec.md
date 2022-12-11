# ChiraEngine File Formats
Unsurprisingly, ChiraEngine does have it's fair share of unique file formats. As such we have dedicated a section of this documentation purely to detailing what these files are and how they are setup alonside any tools you may use to create or modify them.

[TOC]

## Common info
Most of the file formats used by Chira are written in some form of JSON. So they're relatively easy to parse. That being said, there **are** some exceptions to this and any formats that aren't JSON will have this information clearly mentioned.

The pages themselves are formatted in the following structure (headers disabled for better reading experience)

\#\# Example Section
info on section

\#\#\# Section property<br>
`Default Value: <value>`

Specifications:

| Spec | Value      |
|----------|-----------------|
| Type | Example |

## Formats
> NOTE: The following list is based on a currently private branch dedicated to reworking file extensions separate from `editor-extended`

Here is a complete (as of writing) list of ChiraEngine unique formats. Any formats that have pages will have them linked here.
- @subpage cscnspec
- @subpage cmdlspec
- @subpage cmshspec
- CTEX
- CMAT
- CSH

## Tools
Currently there are no external parsers, generators, or editors for any of the formats.