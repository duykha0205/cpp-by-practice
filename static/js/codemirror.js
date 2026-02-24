// CodeMirror factory — wraps CodeMirror 5 (loaded from CDN)
var CodeMirrorFactory = {
    create: function(container, initialCode) {
        if (typeof CodeMirror === 'undefined') {
            // Fallback: plain textarea if CDN failed
            return this._createFallback(container, initialCode);
        }

        var editor = CodeMirror(container, {
            value: initialCode || '',
            mode: 'text/x-c++src',
            theme: 'custom',
            lineNumbers: true,
            matchBrackets: true,
            autoCloseBrackets: true,
            indentUnit: 4,
            tabSize: 4,
            indentWithTabs: false,
            lineWrapping: false,
            viewportMargin: Infinity,
            extraKeys: {
                'Tab': function(cm) {
                    cm.replaceSelection('    ', 'end');
                }
            }
        });

        // Auto-refresh after DOM settles
        setTimeout(function() { editor.refresh(); }, 1);

        return {
            getValue: function() { return editor.getValue(); },
            setValue: function(v) {
                editor.setValue(v);
                setTimeout(function() { editor.refresh(); }, 1);
            }
        };
    },

    _createFallback: function(container, initialCode) {
        var textarea = document.createElement('textarea');
        textarea.className = 'editor-fallback';
        textarea.value = initialCode || '';
        textarea.spellcheck = false;
        textarea.style.cssText =
            'width: 100%; min-height: 150px; padding: 0.75rem;' +
            'font-family: var(--font-mono); font-size: 0.9rem; line-height: 1.5;' +
            'background: var(--bg-editor); color: var(--text-primary);' +
            'border: none; resize: vertical; outline: none; tab-size: 4;';

        textarea.addEventListener('keydown', function(e) {
            if (e.key === 'Tab') {
                e.preventDefault();
                var start = this.selectionStart;
                var end = this.selectionEnd;
                this.value = this.value.substring(0, start) + '    ' + this.value.substring(end);
                this.selectionStart = this.selectionEnd = start + 4;
            }
        });

        container.appendChild(textarea);
        return {
            getValue: function() { return textarea.value; },
            setValue: function(v) { textarea.value = v; }
        };
    }
};
