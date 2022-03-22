$('.ui.container > div.center.aligned:first')
    .visibility({
        once: false,
        onBottomPassed: function() {
            $('.fixed.menu').transition('fade in');
        },
        onBottomPassedReverse: function() {
            $('.fixed.menu').transition('fade out');
        }
    })
;

function f(t) {if(moment(t).diff(moment()) < 0) return "Submission due passed " + moment(t).fromNow(); return "Submission due " + moment(t).fromNow();}
function showTimer(t) {
    moment.relativeTimeThreshold('s', 60);
    moment.relativeTimeThreshold('ss', 10);
    moment.relativeTimeThreshold('m', 60);
    moment.relativeTimeThreshold('h', 24);
    moment.relativeTimeThreshold('d', 365);

    var a = moment(t).format('M/D H:mm:ss');
    function v() {
        $("#timer").html(f(t) + " (Due date: " + a + ")");
    }
    v();
    setInterval(v,1000);
}

if(hljs) {
    document.querySelectorAll('div.ui pre').forEach((block) => {
        hljs.highlightBlock(block);
    });
}

