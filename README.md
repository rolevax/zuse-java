# Zuse Usable Structured Editor

(TODO: introduction video)

Zuse is a code editor that is:

- Structured
- Usable
- Experimental

## Structured?

Code is structure, not text.

A structured code editor is a code editor
that edits the underlying structure instead of the text representation.
A structured code editor
can free you from handling syntax matters, 
and thus improve your coding efficiency.

## Usable?

Today, IDE's are armed with dozens of language-aware features.
but their cores are still plain text editors.
Why? Because we have no choice ---
only text editors are usable, 
and existing structured editors are awkward to use.

However, we believe that text editors are not the end,
and here we are trying to create a better structured editor.

## Experimental?

We set up this project to build a usable structured editor
based on three ideas:

- Use editing-oriented tree to simplify the grammar
- Use stack-managed Vi-like keymap modes to power-up basic operations
- Use macro keymaps to facilitate common operations

These three ideas applies to arbitrary programming languages,
but for ease of sketching,
currently Zuse supports only Java.

## User Manual

(TODO)

## Build

```
$ qmake -config release
$ make 
```

