function doPost(e){
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  var params = e.postData.contents;

  var data = parseQueryString(params);
  var upgradetry = parseInt(data.upgradetry);
  var username = data.username;
  var starttime = data.starttime;
  var endtime = data.endtime;
  var enhancementLevel = parseInt(data.enhancementLevel);
  var trytime = data.trytime;
  var activeNum = data.activeNum;

  // 숫자가 아니면 0을 대입 맞으면 받은 값 대입
  upgradetry = isNaN(upgradetry) ? 0 : upgradetry;
  activeNum = isNaN(activeNum) ? 0 : 1;

  // 데이터를 넣을 행을 정하는 변수
  var nextRow = sheet.getLastRow() + 1;

  // 원래 해당 셀에 있던 데이터를 저장하는 변수
  var oridataA2 = sheet.getRange(sheet.getLastRow(), 1).getValue();
  oridataA2 = isNaN(oridataA2) ? 0 : oridataA2;

  // 셀에 받은 데이터 삽입
  if(activeNum === 0){
    sheet.getRange(nextRow, 1).setValue(oridataA2 + upgradetry);
    sheet.getRange(nextRow, 2).setValue(username);
    sheet.getRange(nextRow, 3).setValue(starttime);
    sheet.getRange(nextRow, 5).setValue(trytime);
    sheet.getRange(nextRow, 6).setValue(enhancementLevel);
  } else{
    sheet.getRange(nextRow, 1).setValue(oridataA2 + upgradetry);
    sheet.getRange(nextRow, 2).setValue(username);
    sheet.getRange(nextRow, 3).setValue(starttime);
    sheet.getRange(nextRow, 4).setValue(endtime);
  }

  return ContentService.createTextOutput("sucess");
}

function parseQueryString(queryString){
  var params = {};
  var queryStringSplit = queryString.split("&");
  for (var i = 0; i < queryStringSplit.length; i++) {
    var temp = queryStringSplit[i].split("=");
    params[decodeURIComponent(temp[0])] = decodeURIComponent(temp[1]);
  }
  return params;
}
