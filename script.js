const panel = document.querySelector('#panel');
document.querySelector('#briefing').addEventListener('click', () => {
  panel.innerHTML = '<p class="panel-label">MISSION BRIEFING</p><h2>Neon run</h2><p>Cross the coast before sunrise. This is fictional placeholder text for the concept interface.</p>';
});
document.querySelector('#status').addEventListener('click', () => {
  panel.innerHTML = '<p class="panel-label">BUILD STATUS</p><h2>Concept v0.0.1</h2><p>Static browser prototype. Not an executable game and not affiliated with any game publisher.</p>';
});
