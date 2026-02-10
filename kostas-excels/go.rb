require "rubyXL"
require "rubyXL/convenience_methods"

left = RubyXL::Parser.parse("./left.xlsx")
right = RubyXL::Parser.parse("./right.xlsx")

sheetNameStart = 2
sheetNameEnd = 29

(sheetNameStart..sheetNameEnd).each do |sheetNameNumber|
  rightValueCol = sheetNameNumber
  leftSheet1 = left[sheetNameNumber.to_s]

  countriesRow = 0
  countriesColStart = 2
  countriesColEnd = leftSheet1.sheet_data[countriesRow].size - 1

  leftCategoriesCol = 1
  leftCategoriesRowStart = 2
  leftCategoriesRowEnd = leftSheet1.sheet_data.rows.size - 1

  leftCategoriesTotalCol = 0
  leftCategoriesTotalRowStart = 2
  leftCategoriesTotalRowEnd = leftSheet1.sheet_data.rows.size - 1

  categoriesTotals = [
    "Total Revenue",
    "Total Reimbursable",
    "Consulting Fee Revenue",
    "Total Direct Expenses",
    "Gross Profit",
    "Total Indirect Expenses",
    "Oper Profit/(Loss) Before Equity",
    "Operating Profit",
    "Total Other (Inc) Exp",
    "Profit/(Loss) Before Tax",
    "Net Profit/(Loss)",
  ]

  # puts sheetName

  (countriesColStart..countriesColEnd).each do |col|
    country = leftSheet1.sheet_data[countriesRow][col].value
    if country == "Total"
      # country = "Europe"
      country = "APAC"
    end

    next if leftSheet1.sheet_data[countriesRow + 1][col].value != "Actual US$"
    puts country

    rightCountrySheet = right[country]
    if rightCountrySheet.nil?
      puts "Sheet not found for " + country
      next
    end

    rightCategoriesCol = 1
    rightCategoriesRowStart = 5
    rightCategoriesRowEnd = 116

    (leftCategoriesRowStart..leftCategoriesRowEnd).each do |row|
      category = leftSheet1.sheet_data[row][leftCategoriesCol].value
      next if category.nil?

      # puts "LEFT " + category

      value = leftSheet1.sheet_data[row][col].value
      next if value.nil?

      puts "  " + category + " " + value.to_s

      ### might be safer to check the "account" caterogy too
      (rightCategoriesRowStart..rightCategoriesRowEnd).each do |rightCategoryRow|
        # puts rightCategoriesCol, rightCategoryRow
        next if rightCountrySheet[rightCategoryRow][rightCategoriesCol].value.nil?
        # puts "BBBB" + rightCountrySheet[rightCategoryRow][rightCategoriesCol].value + country

        if rightCountrySheet[rightCategoryRow][rightCategoriesCol].value == category
          ### add log if *not* moved, instead

          rightCountrySheet[rightCategoryRow][rightValueCol].change_contents(value)
          puts "    Moved"
          break
        end
      end
    end

    (leftCategoriesTotalRowStart..leftCategoriesTotalRowEnd).each do |row|
      category = leftSheet1.sheet_data[row][leftCategoriesTotalCol].value
      next if category.nil?
      next if !categoriesTotals.include? category

      # puts "LEFT " + category

      value = leftSheet1.sheet_data[row][col].value
      next if value.nil?

      puts "  " + category + " " + value.to_s

      ### might be safer to check the "account" caterogy too
      (rightCategoriesRowStart..rightCategoriesRowEnd).each do |rightCategoryRow|
        # puts rightCategoriesCol, rightCategoryRow
        next if rightCountrySheet[rightCategoryRow][rightCategoriesCol].value.nil?
        # puts "BBBB" + rightCountrySheet[rightCategoryRow][rightCategoriesCol].value + country

        if rightCountrySheet[rightCategoryRow][rightCategoriesCol].value == category
          ### add log if *not* moved, instead

          rightCountrySheet[rightCategoryRow][rightValueCol].change_contents(value)
          puts "    Moved"
          break
        end
      end
    end
  end
end

right.write("./right-new.xlsx")
