// CodeMirror factory
// For MVP, we use a styled textarea as the editor.
// To upgrade to full CodeMirror 6, replace this file with the CodeMirror bundle
// and update CodeMirrorFactory.create accordingly.

var CodeMirrorFactory = {
    create: function(container, initialCode) {
        var wrapper = document.createElement('div');
        wrapper.style.position = 'relative';

        var lineNumbers = document.createElement('div');
        lineNumbers.className = 'line-numbers';
        lineNumbers.style.cssText =
            'position: absolute; left: 0; top: 0; bottom: 0; width: 3rem;' +
            'background: var(--bg-secondary); border-right: 1px solid var(--border-color);' +
            'color: var(--text-muted); font-family: var(--font-mono); font-size: 0.85rem;' +
            'line-height: 1.5; padding: 0.75rem 0; text-align: right; user-select: none;' +
            'overflow: hidden;';

        var textarea = document.createElement('textarea');
        textarea.spellcheck = false;
        textarea.autocomplete = 'off';
        textarea.autocapitalize = 'off';
        textarea.value = initialCode || '';
        textarea.style.cssText =
            'width: 100%; min-height: 150px; padding: 0.75rem 0.75rem 0.75rem 3.5rem;' +
            'font-family: var(--font-mono); font-size: 0.9rem; line-height: 1.5;' +
            'background: var(--bg-editor); color: var(--text-primary);' +
            'border: none; resize: vertical; outline: none; tab-size: 4;' +
            'white-space: pre; overflow-wrap: normal; overflow-x: auto;';

        function updateLineNumbers() {
            var lines = textarea.value.split('\n').length;
            var html = '';
            for (var i = 1; i <= lines; i++) {
                html += '<div style="padding: 0 0.5rem;">' + i + '</div>';
            }
            lineNumbers.innerHTML = html;
            lineNumbers.style.height = textarea.scrollHeight + 'px';
        }

        textarea.addEventListener('input', updateLineNumbers);
        textarea.addEventListener('scroll', function() {
            lineNumbers.style.transform = 'translateY(-' + textarea.scrollTop + 'px)';
        });

        textarea.addEventListener('keydown', function(e) {
            if (e.key === 'Tab') {
                e.preventDefault();
                var start = this.selectionStart;
                var end = this.selectionEnd;
                this.value = this.value.substring(0, start) + '    ' + this.value.substring(end);
                this.selectionStart = this.selectionEnd = start + 4;
                updateLineNumbers();
            }
        });

        wrapper.appendChild(lineNumbers);
        wrapper.appendChild(textarea);
        container.appendChild(wrapper);

        updateLineNumbers();

        // Auto-resize
        function autoResize() {
            textarea.style.height = 'auto';
            var computed = Math.max(150, textarea.scrollHeight);
            textarea.style.height = computed + 'px';
        }
        textarea.addEventListener('input', autoResize);
        autoResize();

        return {
            getValue: function() { return textarea.value; },
            setValue: function(v) {
                textarea.value = v;
                updateLineNumbers();
                autoResize();
            }
        };
    }
};
