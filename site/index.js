function submitData(){
    var textBox = document.getElementById("translatedText");
    var input = document.getElementById("myInput");
    var value = input.value;
    console.log(value);
    textBox.value = value;
}
